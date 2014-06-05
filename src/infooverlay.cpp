/* 
* \file infooverlay.cpp
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

#include "infooverlay.h"

namespace sfs_visualizer
{
InfoOverlay::InfoOverlay(InfoProvider& wavefield)
	:_oInfoProv(wavefield)
{
	IsVisible(false);
}

InfoOverlay::~InfoOverlay(void)
{
}

void InfoOverlay::key(unsigned char key, int x, int y)
{
	switch (key) {
		
		case 'i':
			IsVisible(!IsVisible());
			break;
		
		default:
			break;
	}
}

void InfoOverlay::draw_impl()
{
    glLoadIdentity();
    float pos = 0.5f;
    PropertyManager& propManager = PropertyManager::instance();
    for (PropertyMap::const_iterator it = propManager._properties.begin();
        it!=propManager._properties.end();++it)
    {
        if (!it->second.IsVisible())
            continue;
        std::stringstream s;
        s<<it->second.Name() << "=" << it->second.Value();
        drawBitmapText(s.str(),0.5,pos,-0.0001);
        pos-=0.05f;
    }
}

}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='