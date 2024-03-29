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

#include <inviwo/volumejacobian/volumejacobianmodule.h>
#include <inviwo/volumejacobian/processors/volumejacobianoperations.h>
#include <inviwo/volumejacobian/processors/volumejacobianprocessor.h>

namespace inviwo {

VolumeJacobianModule::VolumeJacobianModule(InviwoApplication* app) : InviwoModule(app, "VolumeJacobian") {
    // Add a directory to the search path of the Shadermanager
    // ShaderManager::getPtr()->addShaderSearchPath(getPath(ModulePath::GLSL));

    // Register objects that can be shared with the rest of inviwo here:

    // Processors
    registerProcessor<VolumeJacobianOperations>();
    registerProcessor<VolumeJacobianProcessor>();

    // Properties
    // registerProperty<VolumeJacobianProperty>();

    // Readers and writes
    // registerDataReader(std::make_unique<VolumeJacobianReader>());
    // registerDataWriter(std::make_unique<VolumeJacobianWriter>());

    // Data converters
    // registerRepresentationConverter(std::make_unique<VolumeJacobianDisk2RAMConverter>());

    // Ports
    // registerPort<VolumeJacobianOutport>();
    // registerPort<VolumeJacobianInport>();

    // PropertyWidgets
    // registerPropertyWidget<VolumeJacobianPropertyWidget, VolumeJacobianProperty>("Default");

    // Dialogs
    // registerDialog<VolumeJacobianDialog>(VolumeJacobianOutport);

    // Other things
    // registerCapabilities(std::make_unique<VolumeJacobianCapabilities>());
    // registerSettings(std::make_unique<VolumeJacobianSettings>());
    // registerMetaData(std::make_unique<VolumeJacobianMetaData>());
    // registerPortInspector("VolumeJacobianOutport", "path/workspace.inv");
    // registerProcessorWidget(std::string processorClassName, std::unique_ptr<ProcessorWidget> processorWidget); 
    // registerDrawer(util::make_unique_ptr<VolumeJacobianDrawer>());
}

}  // namespace inviwo
