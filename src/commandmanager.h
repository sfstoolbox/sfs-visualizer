/**
 * \class CommandManager
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

#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "stdincl.h"
#include "Command.h"

namespace sfs_visualizer
{
    /**
     * this class knows all commands and can dispatch userinputs to them                                        
     **/
    class CommandManager
    {
    public:
        CommandManager(void);
        ~CommandManager(void);

        void addCommand(const std::string& name,unsigned char key, const std::string& inputDescription, Property& prop, PropertyCommand::Operation operation,bool provokesUIUpdate=true);
        void addMouseCommand(const std::string& name,unsigned char key, const std::string& inputDescription, Property& prop, PropertyCommand::Operation operation,bool provokesUIUpdate=true);
        void dispatchKey(unsigned char key);
        void dispatchMouse(unsigned char key); 
        //check if a command changed a state -> prevent recalculations
        bool checkNResetDirty(); 

    private:
        bool _bDirty;   

        std::map<unsigned char,PropertyCommand> _keycommands;
        std::map<unsigned char,PropertyCommand> _mousecommands;
    };
}
#endif
// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='