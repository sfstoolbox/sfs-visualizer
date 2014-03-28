#include "commandmanager.h"


namespace sfs_visualizer
{

CommandManager::CommandManager(){}
CommandManager::~CommandManager(){}

void CommandManager::addCommand(const std::string& name,unsigned char key, const std::string& inputDescription,Property& prop, PropertyCommand::Operation operation,bool provokesUIUpdate)
{
   _keycommands.insert(std::pair<unsigned char, PropertyCommand>(key,PropertyCommand(key,inputDescription,name,prop,operation,provokesUIUpdate)));
}

void CommandManager::addMouseCommand( const std::string& name,unsigned char key, const std::string& inputDescription, Property& prop, PropertyCommand::Operation operation,bool provokesUIUpdate)
{
    _mousecommands.insert(std::pair<unsigned char, PropertyCommand>(key,PropertyCommand(key,inputDescription,name,prop,operation,provokesUIUpdate)));
}

 void CommandManager::dispatchKey(unsigned char key)
{
     if (_keycommands.count(key)!=0)
      _bDirty |= _keycommands[key].execute();
}

 void CommandManager::dispatchMouse(unsigned char key)
 {
     if (_mousecommands.count(key)!=0)
       _bDirty |=  _mousecommands[key].execute();
 }

bool CommandManager::checkNResetDirty()
{
    bool result = _bDirty;
    _bDirty = false;
    return result;
}


}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='
