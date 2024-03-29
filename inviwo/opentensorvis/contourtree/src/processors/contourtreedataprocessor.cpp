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

#include <inviwo/contourtree/processors/contourtreedataprocessor.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo ContourTreeDataProcessor::processorInfo_{
    "org.inviwo.ContourTreeDataProcessor",          // Class identifier
    "Contour Tree Data",                  // Display name
    "OpenTensorVis",                                // Category
    CodeState::Experimental,                        // Code state
    "topology, merge tree, split tree, join tree",  // Tags
};
const ProcessorInfo ContourTreeDataProcessor::getProcessorInfo() const { return processorInfo_; }

ContourTreeDataProcessor::ContourTreeDataProcessor()
    : Processor()
    , contourTreeInport_("contourTreeInport")
    , contourTreeOutport_("passthrough")
    , contourTreeDataOutport_("contourTreeDataOutport") {

    addPorts(contourTreeInport_, contourTreeOutport_,contourTreeDataOutport_);
}

void ContourTreeDataProcessor::process() {
    const auto t1 = std::chrono::high_resolution_clock::now();

    contourTreeDataOutport_.setData(
        std::make_shared<contourtree::ContourTreeData>(contourTreeInport_.getData()));

    const auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Contour tree data process(): "
              << std::to_string(
                     std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count())
              << std::endl;

    contourTreeOutport_.setData(contourTreeInport_.getData());
}

}  // namespace inviwo
