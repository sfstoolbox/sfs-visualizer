#include "Property.h"


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
