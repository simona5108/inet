//
// Copyright (C) 2020 OpenSim Ltd.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef __INET_SCENEVISUALIZERBASE_H
#define __INET_SCENEVISUALIZERBASE_H

#include "inet/common/geometry/object/Box.h"
#include "inet/visualizer/base/VisualizerBase.h"

namespace inet {

namespace visualizer {

class INET_API SceneVisualizerBase : public VisualizerBase
{
  protected:
    Coord sceneMin;
    Coord sceneMax;

  protected:
    virtual void initialize(int stage) override;
    virtual Box getSceneBounds();
};

} // namespace visualizer

} // namespace inet

#endif

