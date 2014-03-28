#include "application.h"
#include "stdincl.h"
#include <boost/algorithm/string/replace.hpp>


namespace po = boost::program_options;

namespace sfs_visualizer
{
    Application::Application(void)
        :_oEngine()
    {
    }

    Application::~Application(void)
    {
    }

    Application& Application::instance()
    {
        static Application instance;
        return instance;
    }

    RenderEngine& Application::getEngine()
    {
        return _oEngine;
    }
    void Application::initOptions(int ac, char* av[])
    {

        std::vector<std::string> sentence; 
        po::options_description desc("Options");    
        desc.add_options()       ("help,h", "Print help messages")  
            ("verbose,v", "print words with verbosity")   
            ("matlabfile,m", po::value<std::vector<std::string> >(), "the matlabfilename(s) to be viewed")  
            ("matlabfilefolder,o", po::value<std::string> (),   "the folder containing *.mat files, that will be loaded in alphanumerical order")  
            ("glslfolder,g", po::value<std::string> ()->default_value(getExecutableDir() + "/GLSL/"),   "the folder containing the glsl-files")  
            ("sourcedefinition,s", po::value<std::string> (),   "the csv-file with source definitions") 
            ("colormap,c", po::value<std::string> ()->default_value(getExecutableDir()+"colormap.bmp"),   "a bitmap used for colormapping") 
            ("properties,p", po::value<std::string> (),   "a csv-file with property values") 
            ("gridWidth,W", po::value<int> (),   "the Width of the calculation-grid e.g. 300")  
            ("gridHeight,H", po::value<int> (),   "the Height of the calculation-grid e.g. 300")  
            ("gridDepth,D", po::value<int> (),   "the Depth of the calculation-grid e.g. 300") 
            ("interval,i",po::value<int> (), "updateintervall for animations in milliseconds") 
            ("workgroupsize,w",po::value<int> ()->default_value(8), "Workgroupsize for Compute Shader") 
            ("fullscreen,f", "is the program started in fullscreenmode")  
            ("developmode,d", "the program has developoptions")  
            ; 

        po::store(po::parse_command_line(ac, av,desc), _commandline_options);
        po::notify(_commandline_options);
    }

    std::string Application::getExecutablePath() const
    {
        //this is the only windows specific code
        wchar_t wchar_buffer[MAX_PATH]; 
        GetModuleFileName(NULL, wchar_buffer, MAX_PATH) ;
        char char_buffer[MAX_PATH];
        wcstombs(char_buffer, wchar_buffer, MAX_PATH);

        return std::string(char_buffer);
    }

    std::string Application::getExecutableDir() const
    {
        std::string exePath = getExecutablePath();
        boost::replace_all(exePath, "sfs-visualizer.exe","");
        return exePath;
    }


    int Application::run(int ac, char* av[])
    {
        try
        {
            initOptions(ac, av);
            _oEngine.start();
        }
        catch(std::exception& ex)//global Exceptionhandler
        {
            std::cout << "Error: " << ex.what() << std::endl;
            _oEngine.shutdown();
            exit(-1);
        }
        catch(...) //global Exceptionhandler
        {
            std::cout << "unknown Error" << std::endl;
            _oEngine.shutdown();
            exit(-1);
        }
        return 0;
    }
}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='