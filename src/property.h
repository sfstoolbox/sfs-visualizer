/**
 * \class Property
 *
 * \author Marcus Zepp
 *
 * \version $Revision: 1.0 $
 *
 * \date $Date: 2014/03/26 14:16:20 $
 *
 * Contact: zeppfisj@mailbox.tu-berlin.de
 *
 */

#ifndef PROPERTY_H
#define PROPERTY_H

#include "stdincl.h"
#include "shader.h"

namespace sfs_visualizer
{

/**
 * capsulates a value, that can be modified and saved                                        
 **/
class Property
{
public:
    Property(float value, std::string name,float step, float defaultValue,Shader* shader=NULL);
    Property(): _value(0.0),  _name(NULL),_step(0.0), _defaultValue(0.0),_maxValue(std::numeric_limits<float>::max()),_minValue(-std::numeric_limits<float>::max()),_shader(NULL){}
    ~Property(void);
    float DefaultValue() const { return _defaultValue; }
    void DefaultValue(float val) { _defaultValue = val; }
    float Value() const { return _value; }
    void Value(float val) {
        if (val <=_maxValue && val >= _minValue)
            _value = val; }
    float Max() const { return _maxValue; }
    void Max(float val) { _maxValue = val; }
    float Min() const { return _minValue; }
    void Min(float val) { _minValue = val; }
    std::string Name() const { return _name; }
    void Name(std::string val) { _name = val; }
    float Step() const { return _step; }
    void Step(float val) { _step = val; }
    void writeShaderValue();
    void resetToDefault();
    Shader* getShader(){return _shader;}
    bool IsVisible() const { return _isVisible; }
    void IsVisible(bool val) { _isVisible = val; }
private:
    float _value;
    std::string _name;
    float _step;
    float _defaultValue;
    float _maxValue;
    float _minValue;
    Shader * _shader;
    bool _isVisible;
   
};



}
#endif


// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='