#include "propertymanager.h"
#include "shader.h"
#include "application.h"

namespace sfs_visualizer
{

PropertyManager::PropertyManager()
:_delimiter(',')
{}

PropertyManager::~PropertyManager(){}

PropertyManager& PropertyManager::instance()
{
    static PropertyManager instance;
    return instance;
}

Property& PropertyManager::addProperty( const std::string& name, float defaultValue )
{
    /*Property p(defaultValue,name,1.1,defaultValue);*/
    _properties.insert(PropertyPair(name,Property(defaultValue,name,1.1,defaultValue)));
   

    //_properties.insert(PropertyPair(name,p));
    return _properties[name];
}

Property& PropertyManager::addShaderProperty(const std::string& name, float defaultValue , Shader * _shader )
{
    //2Do update data for loaded properties (connect to shader etc)
    _properties.insert(PropertyPair(name,Property(defaultValue,name,1.1,defaultValue,_shader)));

    return _properties[name];
}

bool PropertyManager::propertyExists(const std::string& name) const
{
return (_properties.count(name)!=0);
}
Property& PropertyManager::getProperty( const std::string& name )
{
    return _properties[name];
}
float PropertyManager::getPropertyValue( const std::string& name )
{
    if (propertyExists(name))
        return _properties[name].Value();
    else
    {
        std::stringstream s;
        s<<"Property '"<<name<<"' not found"<<std::flush;
        throw std::bad_exception(s.str().c_str());
    }
}

void PropertyManager::writeShaderValues(Shader* activeShader)
{
    for (PropertyMap::const_iterator it = _properties.begin();it!=_properties.end();++it)
    {
        Property p = (*it).second;
        if (p.getShader() == activeShader)
            p.writeShaderValue();
       
    }

}

void PropertyManager::save()
{
    std::string strFileName = "properties.csv";
    if (Application::instance().getCommandlineOptions().count("properties") != 0) 
    {
        strFileName = Application::instance().getCommandlineOptions()["properties"].as<std::string >();
    }

     std::cout << "writing properties to '"<< strFileName << "'" <<std::endl;
    std::string line;
    std::ofstream myfile (strFileName);

    if (!myfile.is_open())
        throw std::bad_exception("properties not found");

    for (PropertyMap::const_iterator it = _properties.begin();it!=_properties.end();++it)
    {
        Property p = (*it).second;
        std::stringstream outstream;
        myfile << p.Name() << _delimiter << p.Value() << _delimiter << p.Step() <<std::endl;
        
    }

    myfile.close();

    std::cout <<"properties saved" <<std::endl;
}

void PropertyManager::load()
{
    if (Application::instance().getCommandlineOptions().count("properties") == 0) 
    {
        return;
    }

    std::string strFileName = Application::instance().getCommandlineOptions()["properties"].as<std::string >();
    std::cout << "using properties from '"<< strFileName << "'" << std::endl;
    std::string line;
    std::ifstream myfile (strFileName);
    std::stringstream outstream;
    
    
    if (!myfile.is_open())
        throw std::bad_exception("properties not found");

    int lineCount =0;
    while (std::getline (myfile,line) )
    {
        //read the entries
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, _delimiter)) {
            elems.push_back(item);
        }
        if (elems.size() >= 2) //ignore header line and empty lines
        {
            try
            {
                std::string name =elems[0].c_str();
                if (_properties.count(name)!=0)
                {
                    _properties[name].Value((float) atof(elems[1].c_str()));
                    _properties[name].DefaultValue((float) atof(elems[1].c_str()));
                    if (elems.size() == 3)
                        _properties[name].Step((float) atof(elems[2].c_str()));

                }
            }
            catch (...)
            {
                std::cout <<"warning: something went wrong while parsing the properties" << std::endl;
            }
        }
    
    }


    myfile.close();
    
    std::cout <<"properties loaded" <<std::endl;
}

void PropertyManager::setPropertyValue( const std::string& name, float val )
{
    _properties[name].Value(val);
}

void PropertyManager::resetPropertyValue( const std::string& name )
{
    _properties[name].resetToDefault();
}





}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='