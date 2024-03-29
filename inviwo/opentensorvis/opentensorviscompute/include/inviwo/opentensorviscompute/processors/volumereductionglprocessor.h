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

#pragma once

#include <inviwo/opentensorviscompute/opentensorviscomputemoduledefine.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/opentensorviscompute/algorithm/volumereductiongl.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/optionproperty.h>

namespace inviwo {

/** \docpage{org.inviwo.VolumeReductionGLProcessor, Volume Reduction Processor}
 * ![](org.inviwo.VolumeReductionGLProcessor.png?classIdentifier=org.inviwo.VolumeReductionGLProcessor)
 *
 * GL implementation of add, min, and max reductions for 3D textures (volumes).
 *
 * ### Inputs
 *   * __Volume inport__ Input volume.
 *
 * ### Outports
 *   * __Volume outport__ Reduced volume.
 *
 * ### Properties
 *   * __Reduction operator__ Operator for reduction.
 *
 */
class IVW_MODULE_OPENTENSORVISCOMPUTE_API VolumeReductionGLProcessor : public Processor {
public:
    VolumeReductionGLProcessor();
    virtual ~VolumeReductionGLProcessor() = default;

    virtual void process() override;

    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

private:
    VolumeInport volumeInport_;
    VolumeOutport volumeOutport_;

    TemplateOptionProperty<ReductionOperator> reductionOperator_;

    VolumeReductionGL gpuReduction_;
};

}  // namespace inviwo
