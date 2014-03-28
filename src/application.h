/**
 * \class Application
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

#ifndef APPLICATION_H
#define APPLICATION_H
#include "renderengine.h"
#include <boost/program_options.hpp>

namespace sfs_visualizer
{
    /*
    * The mainclass of the application, implemented as Singleton
    */
    class Application
    {
    public:

        ~Application(void);
        int run(int ac, char* av[]);

        static Application& instance();//Singleton

        RenderEngine& getEngine();
        boost::program_options::variables_map getCommandlineOptions() const { return _commandline_options; }
        std::string getExecutablePath() const;
        std::string getExecutableDir() const;

    private:
        Application(void);
        Application(const Application&);
        void initOptions(int ac, char* av[]);
        RenderEngine _oEngine;
        boost::program_options::variables_map _commandline_options;

    };

}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='
