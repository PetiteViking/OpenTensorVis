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

#include <inviwo/contourexplorer/contourexplorermoduledefine.h>
#include <inviwo/core/datastructures/bitset.h>
#include <inviwo/core/datastructures/tfprimitiveset.h>
#include <inviwo/core/util/colorbrewer.h>

namespace inviwo {
class IVW_MODULE_CONTOUREXPLORER_API SegmentationColorHelper {
public:
    static std::vector<std::tuple<double, bool, int>> getPositionsAndLevelsForNSegments(size_t n, double slope = std::numeric_limits<double>::epsilon());

    static std::vector<dvec4> getColorMapForNSegments(size_t n);

    static TFPrimitiveSet generateTFPrimitivesForSegments(
        const BitSet& selection, size_t numberOfSegments,
        double slope = std::numeric_limits<double>::epsilon(), const vec4& shadeColor = vec4(0.0f));

    static dvec3 hclToRgb(const dvec3& hcl);

    static dvec3 rgbToHcl(const dvec3& rgb);
};

}  // namespace inviwo