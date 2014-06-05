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
