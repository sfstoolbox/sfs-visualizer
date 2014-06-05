/* 
* \file property.cpp
*
* \author Marcus Zepp
*
* \version $Revision: 1.0 $
*
* \date $Date: 2014/03/26 14:16:20 $
*
* Contact: zeppfisj@mailbox.tu-berlin.de
*
* This file is part of sfs-visualizer.
*
* sfs-visualizer is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* sfs-visualizer is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with sfs-visualizer. If not, see <http://www.gnu.org/licenses/>.
*/

#include "property.h"


namespace sfs_visualizer
{

    Property::Property(float value, std::string name,float step, float defaultValue,Shader* shader)
        :_value(value),_name(name),_step(step),_defaultValue(defaultValue),_maxValue(std::numeric_limits<float>::max()),_minValue(-std::numeric_limits<float>::max()),_shader(shader),_isVisible(true)
    {
    }

    Property::~Property(void)
    {
        _shader = NULL;
    }

    void Property::writeShaderValue()
    {
        if (_shader != NULL && _shader->isActive())
            _shader->setUniform(_name,_value);
    }

    void Property::resetToDefault()
    {
        _value = _defaultValue;
    }
}


// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='
