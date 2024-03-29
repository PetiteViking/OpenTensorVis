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

#include <inviwo/contourexplorer/processors/volumelabelselectionprocessor.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/network/networklock.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo VolumeLabelSelectionProcessor::processorInfo_{
    "org.inviwo.VolumeLabelSelectionProcessor",  // Class identifier
    "Volume Label Selection",                    // Display name
    "OpenTensorVis",                             // Category
    CodeState::Experimental,                     // Code state
    Tags::CPU,                                   // Tags
};
const ProcessorInfo VolumeLabelSelectionProcessor::getProcessorInfo() const {
    return processorInfo_;
}

VolumeLabelSelectionProcessor::VolumeLabelSelectionProcessor()
    : Processor()
    , volumeInport_("volumeInport")
    , brushingAndLinkingInport_("linkingOutport")
    , labels_("labels", "Labels") {

    addPorts(volumeInport_, brushingAndLinkingInport_);
    addProperties(labels_);

    volumeInport_.onChange([this]() { updateList(); });
    brushingAndLinkingInport_.onChange([this]() { updateSelectionExternal(); });
}

void VolumeLabelSelectionProcessor::updateList() {
    if (!volumeInport_.hasData() || !volumeInport_.getData()) {
        return;
    }

    NetworkLock l;

    auto inputVolume = volumeInport_.getData();

    std::vector<bool> currentSelection;
    for (const auto prop : labels_.getPropertiesByType<BoolProperty>()) {
        currentSelection.push_back(prop->get());
    }

    labels_.clear();

    const auto min = static_cast<int32_t>(inputVolume->dataMap_.dataRange.x);
    const auto max = static_cast<int32_t>(inputVolume->dataMap_.dataRange.y);

    currentSelection.resize(max + 1, false);

    BitSet s;

    for (size_t i{0}; i < currentSelection.size(); ++i) {
        if (currentSelection[i]) {
            s.add(i);
        }
    }

    for (int32_t i{min}; i <= max; i++) {
        auto prop = new BoolProperty(std::to_string(i), std::to_string(i), currentSelection[i]);
        prop->onChange([this]() { updateSelectionInternal(); });
        labels_.addProperty(prop);
    }

    brushingAndLinkingInport_.select(s);
}

void VolumeLabelSelectionProcessor::process() {}

void VolumeLabelSelectionProcessor::updateSelectionInternal() {
    auto props = labels_.getProperties();
    BitSet selection;
    for (auto prop : props) {
        auto boolProperty = dynamic_cast<BoolProperty*>(prop);
        if (boolProperty->get()) {
            selection.add(static_cast<size_t>(std::atoi(prop->getIdentifier().c_str())));
        }
    }
    brushingAndLinkingInport_.select(selection);
}

void VolumeLabelSelectionProcessor::updateSelectionExternal() {
    auto props = labels_.getProperties();

    auto selection = brushingAndLinkingInport_.getSelectedIndices();

    for (size_t i{0}; i < props.size(); i++) {
        auto boolProperty = dynamic_cast<BoolProperty*>(props[i]);
        boolProperty->set(selection.contains(i));
    }

    invalidate(InvalidationLevel::Valid);
}

}  // namespace inviwo
