/* 
* \file command.cpp
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

#include "command.h"


namespace sfs_visualizer
{

    Command::Command(unsigned char key,const std::string& inputDescription,const std::string& name)
        :_key(key),_inputDescription(inputDescription),_name(name)
    {
    }

    Command::~Command(void){}

    PropertyCommand::PropertyCommand(unsigned char key, const std::string& inputDescription, const std::string& name,Property& property,Operation operation,bool provokesUIUpdate)
        :Command(key,inputDescription,name),_provokesUIUpdate(provokesUIUpdate),_property(property),_operation(operation)
    {

    }

    bool PropertyCommand::execute()
    {
        float valueBefore = _property.Value();

        switch(_operation)
        {
            
        case Add:
            _property.Value(_property.Value()+_property.Step());
            break;
        case Mul:
            _property.Value(_property.Value()*_property.Step());
            break;
        case Sub:
            _property.Value(_property.Value()-_property.Step());
            break;
        case Div:
            _property.Value(_property.Value()/_property.Step());
            break;
        case Toggle:
           if( _property.Value()==0.0f)
                _property.Value(1.0f);
            else
                _property.Value(0.0f);
            break;
        }

       

//_info << "NSLIZES=" << NSLIZES;
printf("                                                 \r");
std::cout << _property.Name()<< "="<<_property.Value();

//printf("s% %i                              \r",_property.Name(),_property.Value());

if (valueBefore != _property.Value() && _provokesUIUpdate)
    return true;
else
    return false;

    }

    PropertyCommand::~PropertyCommand(void){}
}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='