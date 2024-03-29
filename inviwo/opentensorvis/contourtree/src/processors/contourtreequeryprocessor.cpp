/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2021 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <execution>
#include <inviwo/contourtree/processors/contourtreequeryprocessor.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/network/networklock.h>
#include <inviwo/contourtree/util/util.h>
#include <Persistence.h>
#include <Grid3D.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo ContourTreeQueryProcessor::processorInfo_{
    "org.inviwo.ContourTreeQueryProcessor",         // Class identifier
    "Contour Tree Query",                           // Display name
    "OpenTensorVis",                                // Category
    CodeState::Experimental,                        // Code state
    "topology, merge tree, join tree, split tree",  // Tags
};
const ProcessorInfo ContourTreeQueryProcessor::getProcessorInfo() const { return processorInfo_; }

ContourTreeQueryProcessor::ContourTreeQueryProcessor()
    : Processor()
    , volumeInport_("volumeInport")
    , contourTreeInport_("contourTreeInport")
    , contourTreeDataInport_("contourTreeDataInport")
    , contourTreeSimplificationInport_("contourTreeSimplificationInport")
    , contourTreeTopologicalFeatuesInport_("contourTreeTopologicalFeatuesInport")
    , brushingAndLinkingInport_("brushingAndLinkingInport")
    , voxelizedVolumeOutport_("voxelizedVolumeOutport")
    , smoothVolumeOutport_("smoothVolumeOutport")
    , extremalPointsOutport_("segmentMinimaOutport")
    , queryMethod_("queryMethod", "Query method",
                   {{"option_methodTopoAngler", "TopoAngler", QueryMethod::TopoAngler},
                    {"option_methodCutoff", "Cutoff", QueryMethod::Cutoff},
                    {"option_methodLeaves", "Leaves", QueryMethod::Leaves}},
                   0)
    , methodTopoAngler_("methodTopoAngler", "TopoAngler")

    , queryCriterion_("queryCriterion", "Query criterion",
                      {{"topk", "Top k features", QueryCriterion::TopKFeatures},
                       {"threshold", "Threshold", QueryCriterion::Threshold}},
                      0)
    , topKFeatures_("topKFeatures", "Top k features", 1, 1, 12, 1)
    , threshold_("threshold", "Threshold", 0.0f, 0.0f, 1.0f, 0.0001f)
    , methodCutoff_("methodCutoff", "Cutoff")
    , cutoff_("cutoff", "Function value")
    , simplificationThreshold1_("simplificationThreshold1", "Simplification threshold")
    , numberOfIntersectingArcs_("numberOfIntersectingArcs", "Number of features")
    , methodNLeaves_("methodNLeaves", "N Leaves")
    , nLeaves_("nLeaves", "Number of leaves", 1, 1, 12, 1)
    , simplificationThreshold2_("simplificationThreshold2", "Simplification threshold")
    , text_("text", "Contour tree info", "", InvalidationLevel::InvalidOutput,
            PropertySemantics::TextEditor) {
    addPorts(volumeInport_, contourTreeInport_, contourTreeDataInport_,
             contourTreeSimplificationInport_, contourTreeTopologicalFeatuesInport_,
             brushingAndLinkingInport_, voxelizedVolumeOutport_, smoothVolumeOutport_,
             extremalPointsOutport_);

    addProperties(queryMethod_, methodTopoAngler_, methodCutoff_, methodNLeaves_, text_);

    contourTreeSimplificationInport_.onChange([this]() {
        const auto contourTreeSimplification = contourTreeSimplificationInport_.getData();

        NetworkLock l;

        const auto min = contourTreeSimplification->simFn->getMinValue();
        const auto max = contourTreeSimplification->simFn->getMaxValue();

        if (simplificationThreshold1_.get() < min && simplificationThreshold1_.get() > max) {
            simplificationThreshold1_.set(min);
        }

        simplificationThreshold1_.setMinValue(min);
        simplificationThreshold1_.setMaxValue(max);

        if (simplificationThreshold2_.get() < min && simplificationThreshold2_.get() > max) {
            simplificationThreshold2_.set(min);
        }

        simplificationThreshold2_.setMinValue(min);
        simplificationThreshold2_.setMaxValue(max);
    });

    volumeInport_.onChange([this]() {
        NetworkLock l;

        const auto inputVolume = volumeInport_.getData();
        const auto min = static_cast<float>(inputVolume->dataMap_.dataRange.x);
        const auto max = static_cast<float>(inputVolume->dataMap_.dataRange.y);

        if (cutoff_.get() < min && cutoff_.get() > max) {
            cutoff_.set(min);
        }

        cutoff_.setMinValue(min);
        cutoff_.setMaxValue(max);
    });

    /**
     * Setup for TopoAngler's method
     */
    methodTopoAngler_.visibilityDependsOn(queryMethod_,
                                          [](const TemplateOptionProperty<QueryMethod>& p) {
                                              return p.get() == QueryMethod::TopoAngler;
                                          });
    topKFeatures_.visibilityDependsOn(queryCriterion_,
                                      [](const TemplateOptionProperty<QueryCriterion>& p) {
                                          return p.get() == QueryCriterion::TopKFeatures;
                                      });
    threshold_.visibilityDependsOn(queryCriterion_,
                                   [](const TemplateOptionProperty<QueryCriterion>& p) {
                                       return p.get() == QueryCriterion::Threshold;
                                   });

    methodTopoAngler_.addProperties(queryCriterion_, topKFeatures_, threshold_);

    /**
     * Setup for Cutoff method
     */
    methodCutoff_.visibilityDependsOn(queryMethod_,
                                      [](const TemplateOptionProperty<QueryMethod>& p) {
                                          return p.get() == QueryMethod::Cutoff;
                                      });
    numberOfIntersectingArcs_.setReadOnly(true);
    methodCutoff_.addProperties(cutoff_, simplificationThreshold1_, numberOfIntersectingArcs_);

    /**
     * Setup for nLeaves method
     */
    methodNLeaves_.visibilityDependsOn(queryMethod_,
                                       [](const TemplateOptionProperty<QueryMethod>& p) {
                                           return p.get() == QueryMethod::Leaves;
                                       });
    methodNLeaves_.addProperties(nLeaves_, simplificationThreshold2_);
}

void ContourTreeQueryProcessor::process() {
    if (brushingAndLinkingInport_.isSelectionModified())
        return;  // If the selection is modified then that is what triggered the evaluation. In that
                 // case we don't need to to anything.

    const auto t1 = std::chrono::high_resolution_clock::now();

    switch (queryMethod_.get()) {
        case QueryMethod::TopoAngler:
            queryTopoAngler();
            break;
        case QueryMethod::Cutoff:
            queryCutoff();
            break;
        case QueryMethod::Leaves:
            queryNLeaves();
            break;
    }

    const auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Contour tree query process(): "
              << std::to_string(
                     std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count())
              << std::endl;

    brushingAndLinkingInport_.select({});
}

void ContourTreeQueryProcessor::queryTopoAngler() {
    std::cout << "Entering ContourTreeQueryProcessor::queryTopoAngler()" << std::endl;

    if (!util::checkPorts(contourTreeInport_, contourTreeTopologicalFeatuesInport_)) return;

    const auto contourTree = contourTreeInport_.getData();
    const auto topologicalFeatures = contourTreeTopologicalFeatuesInport_.getData();

    std::pair<contourtree::SimplifyCT, std::vector<contourtree::Feature>> features;

    const auto topKFeatures =
        queryCriterion_.get() == QueryCriterion::TopKFeatures ? topKFeatures_.get() : -1;

    if (!topologicalFeatures->isPartitioned) {
        features = topologicalFeatures->getArcFeatures(topKFeatures, threshold_.get());

    } else {
        features =
            topologicalFeatures->getPartitionedExtremaFeatures(topKFeatures, threshold_.get());
    }

    const auto extremalPoints = initExtremalPoints(features.second.size());
    auto& extremalPointsRef = *extremalPoints;

    /*
     * Look up which feature the arcId in arcMap belongs to and assign value. That should be it.
     */
    const auto numberOfElements = contourTree->noVertices;

    const auto rawData = new uint16_t[numberOfElements];
    std::fill_n(rawData, numberOfElements, 0);

    for (size_t i{1}; i < features.second.size(); ++i) {
        for (const auto arcId : features.second[i].arcs) {
            for (int64_t j{0}; j < numberOfElements; ++j) {
                if (contourTree->arcMap[j] == arcId) {
                    rawData[j] = static_cast<uint16_t>(i);

                    extremalPointsRef[i] =
                        std::min(extremalPointsRef[i], contourTree->data->getFunctionValue(j));
                }
            }
        }
    }

    // We know that the 0th feature contains the global minimum (I think?)
    extremalPointsRef[0] = *std::min_element(rawData, rawData + numberOfElements);

    generateSegmentationVolume(rawData, features.second.size());

    updateContourTreeInfo(features.first);

    extremalPointsOutport_.setData(extremalPoints);

    LogInfo(fmt::format("{}", *extremalPoints));
}

void ContourTreeQueryProcessor::queryCutoff() {
    std::cout << "Entering ContourTreeQueryProcessor::queryCutoff()" << std::endl;

    if (!util::checkPorts(contourTreeDataInport_, volumeInport_,
                          contourTreeTopologicalFeatuesInport_))
        return;

    const auto cutoff = cutoff_.get();

    const auto inputVolume = volumeInport_.getData();

    inputVolume->getRepresentation<VolumeRAM>()->dispatch<void, dispatching::filter::Scalars>(
        [&, this](auto vrPrecision) {
            using ValueType = util::PrecisionValueType<decltype(vrPrecision)>;
            auto volumeData = vrPrecision->getDataTyped();

            const auto contourTreeData = contourTreeDataInport_.getData();
            const auto topologicalFeatures = contourTreeTopologicalFeatuesInport_.getData();

            contourtree::SimplifyCT sim;
            sim.setInput(contourTreeData);

            sim.simplify(topologicalFeatures->order, -1, simplificationThreshold1_.get(),
                         topologicalFeatures->weights);

            auto inRange = [&](const ValueType a, const ValueType b) {
                return cutoff >= static_cast<float>(a) && cutoff <= static_cast<float>(b);
            };

            std::vector<size_t> intersectingBranches{};

            updateContourTreeInfo(sim);

            const auto& branches = sim.branches;
            const auto& arcs = contourTreeData->arcs;
            const auto& arcMap = contourTreeInport_.getData()->arcMap;

            for (size_t i{0}; i < branches.size(); ++i) {
                const auto value1 = volumeData[contourTreeData->nodeVerts[branches[i].from]];
                const auto value2 = volumeData[contourTreeData->nodeVerts[branches[i].to]];

                if (inRange(value1, value2)) {
                    intersectingBranches.push_back(i);
                }
            }

            const auto extremalPoints = initExtremalPoints(intersectingBranches.size());
            auto& extremalPointsRef = *extremalPoints;

            const auto numberOfElements = contourTreeInport_.getData()->noVertices;
            const auto rawData = new uint16_t[numberOfElements];
            std::fill_n(rawData, numberOfElements, 0);

            // DEBUG
            // Return true if the map is faulty
            auto checkArcMap = [](const std::vector<uint32_t>& map) -> bool {
                std::unordered_map<unsigned, int> s;

                for (const auto id : map) {
                    s[id]++;
                }

                auto it = std::find_if(std::begin(s), std::end(s),
                                       [](const auto& valuePair) { return valuePair.second == 1; });

                return it == std::end(s);
            };

            if (checkArcMap(arcMap)) {
                LogError("Faulty arc map.");
            }

            if (intersectingBranches.size() > 100) {
                std::cout << "More than 100 features selected, please review settings."
                          << std::endl;
                return;
            }

            std::cout << "Segmenting volume data" << std::endl;

            for (size_t i{0}; i < intersectingBranches.size(); ++i) {
                const auto branchIndex = intersectingBranches[i];
                const auto& branch = branches[branchIndex];

                for (size_t j{0}; j < arcMap.size(); ++j) {
                    for (const auto arcId : branch.arcs_) {
                        if (arcId == arcMap[j]) {
                            // At this point we know that this voxel belongs to this arc
                            if (static_cast<float>(volumeData[j]) <= cutoff) {
                                rawData[j] = i;
                                extremalPointsRef[i] = std::min(extremalPointsRef[i],
                                                                static_cast<float>(volumeData[j]));
                                break;
                            }
                        }
                    }
                }

                // Not sure if Branch stores all arcs it contains or just the simplification level
                // below it. For safety, let's recurse.
                std::function<void(std::vector<uint32_t>)> recurse;

                recurse = [&](const std::vector<uint32_t>& children) {
                    for (const auto childIndex : children) {
                        const auto& childBranch = branches[childIndex];
                        for (size_t j{0}; j < arcMap.size(); ++j) {
                            for (const auto arcId : childBranch.arcs_) {
                                if (arcId == arcMap[j]) {
                                    rawData[j] = i;
                                    extremalPointsRef[i] = std::min(
                                        extremalPointsRef[i], static_cast<float>(volumeData[j]));
                                }
                            }
                        }

                        recurse(childBranch.children);
                    }
                };

                recurse(branch.children);
            }

            

            numberOfIntersectingArcs_.set(intersectingBranches.size());
            LogInfo(fmt::format("{}", *extremalPoints));
            extremalPointsOutport_.setData(extremalPoints);
            generateSegmentationVolume(rawData, intersectingBranches.size());
        });
}

void ContourTreeQueryProcessor::queryNLeaves() {
    std::cout << "Entering ContourTreeQueryProcessor::queryNLeaves()" << std::endl;

    if (!util::checkPorts(volumeInport_, contourTreeInport_, contourTreeDataInport_,
                          contourTreeSimplificationInport_))
        return;

    const auto contourTree = contourTreeInport_.getData();
    const auto contourTreeData = contourTreeDataInport_.getData();
    const auto contourTreeSimplification = contourTreeSimplificationInport_.getData();

    const auto topologicalFeatures = contourTreeTopologicalFeatuesInport_.getData();

    if (topologicalFeatures->isPartitioned) {
        
    }

    const auto [simplification, features] =
        volumeInport_.getData()
            ->getRepresentation<VolumeRAM>()
            ->dispatch<std::pair<contourtree::SimplifyCT, std::vector<contourtree::Feature>>,
                       dispatching::filter::Scalars>([&, this](auto vrPrecision) {
                using ValueType = util::PrecisionValueType<decltype(vrPrecision)>;

                const auto grid = std::dynamic_pointer_cast<const contourtree::Grid3D<ValueType>>(
                    contourTree->data);

                std::pair<contourtree::SimplifyCT, std::vector<contourtree::Feature>> f;

                if (contourTree->treeType_ == contourtree::TreeType::JoinTree) {
                    std::cout << "Getting n leaves for JoinTree" << std::endl;
                    f = topologicalFeatures
                            ->getNExtremalArcFeatures<contourtree::TreeType::JoinTree, ValueType>(
                                nLeaves_, simplificationThreshold2_.get(), grid->fnVals);
                }

                if (contourTree->treeType_ == contourtree::TreeType::SplitTree) {
                    std::cout << "Getting n leaves for SplitTree" << std::endl;
                    f = topologicalFeatures
                            ->getNExtremalArcFeatures<contourtree::TreeType::SplitTree, ValueType>(
                                nLeaves_, simplificationThreshold2_.get(), grid->fnVals);
                }

                if (f.second.size() < nLeaves_.get()) {
                    LogWarn(fmt::format(
                        "With persistence simplification level at {} the tree yielded {} features "
                        "instead of the requested {}.",
                        simplificationThreshold2_.get(), f.second.size(), nLeaves_.get()));
                }

                return f;
            });

    updateContourTreeInfo(simplification);

    const auto numberOfElements = contourTree->noVertices;

    const auto rawData = new uint16_t[numberOfElements];
    std::fill_n(rawData, numberOfElements, 0);

    const auto extremalPoints = initExtremalPoints(features.size() + 1);
    auto& extremalPointsRef = *extremalPoints;
    auto& arcMap = contourTree->arcMap;
    const auto data = contourTree->data;

    std::cout << "Segmenting volume data" << std::endl;

    /*std::vector<int> featureIndices(features.size());
    std::iota(std::begin(featureIndices), std::end(featureIndices), 0);*/
    
#ifdef OLDCODE
    for (size_t i{1}; i <= features.size(); i++) {
        for (const auto arcId : features[i - 1].arcs) {
            for (int64_t j{0}; j < numberOfElements; ++j) {
                if (arcMap[j] == arcId) {
                    rawData[j] = static_cast<uint16_t>(i);
                    extremalPointsRef[i] =
                        std::min(extremalPointsRef[i], data->getFunctionValue(j));
                }
            }
        }
    }
#else
    if (features.size()<3){
        for (size_t i{1}; i <= features.size(); i++) {
            std::for_each(std::execution::par, std::begin(features[i - 1].arcs),
                          std::end(features[i - 1].arcs), [&](const auto arcId) {
                              for (int64_t j{0}; j < numberOfElements; ++j) {
                                  if (arcMap[j] == arcId) {
                                      rawData[j] = static_cast<uint16_t>(i);
                                      extremalPointsRef[i] =
                                          std::min(extremalPointsRef[i], data->getFunctionValue(j));
                                  }
                              }
                          });
        } 
    } else {
        std::vector<int> indices(features.size());
        std::iota(std::begin(indices), std::end(indices), 1);

        std::for_each(std::execution::par_unseq, std::begin(indices), std::end(indices),
                      [&](const auto i) {
                          for (const auto arcId : features[i - 1].arcs) {
                              for (int64_t j{0}; j < numberOfElements; ++j) {
                                  if (arcMap[j] == arcId) {
                                      rawData[j] = static_cast<uint16_t>(i);
                                      extremalPointsRef[i] =
                                          std::min(extremalPointsRef[i], data->getFunctionValue(j));
                                  }
                              }
                          }
                      });
    }
#endif

    std::cout << "Done." << std::endl;

    LogInfo(fmt::format("{}", *extremalPoints));
    extremalPointsOutport_.setData(extremalPoints);
    generateSegmentationVolume(rawData, features.size() + 1);
}

void ContourTreeQueryProcessor::generateSegmentationVolume(uint16_t* rawData, const size_t n) {
    std::cout << "Entering ContourTreeQueryProcessor::generateSegmentationVolume()" << std::endl;

    const auto inputVolume = volumeInport_.getData();
    const auto contourTree = contourTreeInport_.getData();

    const auto segmentationVolumeRamPrecision =
        std::make_shared<VolumeRAMPrecision<uint16_t>>(rawData, inputVolume->getDimensions());

    const auto segmentationVolume = std::make_shared<Volume>(segmentationVolumeRamPrecision);

    segmentationVolume->dataMap_.dataRange = segmentationVolume->dataMap_.valueRange =
        dvec2{0, n - 1};
    segmentationVolume->setBasis(inputVolume->getBasis());
    segmentationVolume->setOffset(inputVolume->getOffset());

    const auto smoothVolume = segmentationVolume->clone();
    smoothVolume->setInterpolation(InterpolationType::Linear);

    segmentationVolume->setInterpolation(InterpolationType::Nearest);

    voxelizedVolumeOutport_.setData(segmentationVolume);
    smoothVolumeOutport_.setData(smoothVolume);
}

void ContourTreeQueryProcessor::updateContourTreeInfo(const contourtree::SimplifyCT& simplifyCt) {
    NetworkLock l;
    const auto numberOfNodes = simplifyCt.nodes.size();
    const auto numberOfBranches = simplifyCt.branches.size();
    if (auto simFn = simplifyCt.simFn) {
        const auto minValue = simplifyCt.simFn->getMinValue();
        const auto maxValue = simplifyCt.simFn->getMaxValue();

        text_.set(
            fmt::format("Contour tree info:\nNumber of nodes:{:>30}\nNumber of "
                        "branches:{:>30}\nMin value:{:>30}\nMax value:{:>30}",
                        numberOfNodes, numberOfBranches, minValue, maxValue));

        return;
    }

    text_.set(
        fmt::format("Contour tree info:\nNumber of nodes:{:>30}\nNumber of "
                    "branches:{:>30}",
                    numberOfNodes, numberOfBranches));
}

std::shared_ptr<std::map<size_t, float>> ContourTreeQueryProcessor::initExtremalPoints(
    const size_t n) const {
    auto m = std::make_shared<std::map<size_t, float>>();
    auto& mRef = *m;

    for (size_t i{0}; i < n; i++) {
        mRef[i] = std::numeric_limits<float>::max();
    }

    return m;
}
}  // namespace inviwo
