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