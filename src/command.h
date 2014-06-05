/**
 * \class Command
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

#ifndef COMMAND_H
#define COMMAND_H

#include "stdincl.h"
#include "property.h"

namespace sfs_visualizer
{

 /**
  * implements the command-pattern, that abstracts actions, that can be executed                           
  **/
class Command
{
public:
    Command():
        _name(""),_key(0)
    {}

    Command(unsigned char key,const std::string& inputDescription,const std::string& name);
    ~Command(void);
   
    virtual bool execute() = 0;

    char Key() const { return _key; }
    void Key(char val) { _key = val; }

private:
    
    std::string _name;
    unsigned char _key;
    std::string _inputDescription;
};

/**
 *  This Command can change Property values with defined actions eg. Addition, Subtraction etc.                                       
 **/
class PropertyCommand : public Command
{

public:
    enum Operation{Add,Sub,Mul,Div,Reset,Toggle};

    PropertyCommand():
        _provokesUIUpdate(false),
        _property(Property()),
        _operation()
    {};
    PropertyCommand(unsigned char key,const std::string& inputDescription,const std::string& name,Property& property,PropertyCommand::Operation operation,bool provokesUIUpdate=false);
    ~PropertyCommand(void);
  
    bool execute();

private:

    bool _provokesUIUpdate;
    Property& _property;
    Operation _operation;
};

}
#endif


// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='