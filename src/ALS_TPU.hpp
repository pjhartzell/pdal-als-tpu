/******************************************************************************
 * Copyright (c) 2021, Preston J Hartzell (preston.hartzell@gmail.com)
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following
 * conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Hobu, Inc. nor the
 *       names of its contributors may be used to endorse or promote
 *       products derived from this software without specific prior
 *       written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 ****************************************************************************/

#pragma once

#include <pdal/Filter.hpp>

namespace pdal {

class PDAL_DLL ALS_TPU : public Filter {
    public:
        ALS_TPU() : Filter(), m_cloud(nullptr), m_complete(false)
        {}
        std::string getName() const;

    private:
        double m_sLidarDistance;
        double m_sScanAngle;
        double m_sSensorXY;
        double m_sSensorZ;
        double m_sSensorRollPitch;
        double m_sSensorYaw;
        double m_laserBeamDivergence;
        double m_maximumIncidenceAngle;
        bool m_includeIncidenceAngle;

        virtual void addArgs(ProgramArgs& args);
        virtual PointViewSet run(PointViewPtr view);
        virtual void done(PointTableRef _);

        PointViewPtr tpu(PointViewPtr cloud, PointViewPtr trajectory);
        bool linearInterpolation(
            double t,
            double& ix, double& iy, double& iz, double& ih,
            PointViewPtr trajectory, PointId& interpIdx);

        PointViewPtr m_cloud;
        bool m_complete;

        // Temp function for debugging
        void savePoints(std::string filename, PointViewPtr view);

        ALS_TPU& operator=(const ALS_TPU&);     // not implemented
        ALS_TPU(const ALS_TPU&);                // not implemented
};

} // namespace pdal
