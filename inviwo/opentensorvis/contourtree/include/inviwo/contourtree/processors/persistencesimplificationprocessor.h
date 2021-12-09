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

#include <inviwo/contourtree/contourtreemoduledefine.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/contourtree/ports/contourtreedataport.h>
#include <inviwo/contourtree/ports/contourtreesimplificationport.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <Persistence.h>

namespace inviwo {

/** \docpage{org.inviwo.PersistenceSimplificationProcessor, Persistence Simplification Processor}
 * ![](org.inviwo.PersistenceSimplificationProcessor.png?classIdentifier=org.inviwo.PersistenceSimplificationProcessor)
 * Explanation of how to use the processor.
 *
 * ### Inports
 *   * __<Inport1>__ <description>.
 *
 * ### Outports
 *   * __<Outport1>__ <description>.
 *
 * ### Properties
 *   * __<Prop1>__ <description>.
 *   * __<Prop2>__ <description>
 */
class IVW_MODULE_CONTOURTREE_API PersistenceSimplificationProcessor : public Processor {
public:
    PersistenceSimplificationProcessor();
    virtual ~PersistenceSimplificationProcessor() = default;

    virtual void process() override;

    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

private:
    ContourTreeDataInport contourTreeDataInport_;
    ContourTreeSimplificationOutport contourTreeSimplificationOutport_;

    FloatProperty persistence_;

    std::shared_ptr<contourtree::Persistence> persistenceObject_;
    std::shared_ptr<contourtree::SimplifyCT> simplifyCt_;
};

}  // namespace inviwo
