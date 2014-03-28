/**
 * \class InfoProvider
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

#ifndef INFOPROVIDER_H
#define INFOPROVIDER_H
#include "stdincl.h"
#include "propertymanager.h"

namespace sfs_visualizer
{

     /**
     * Interface for objects, that want infos to be displayed in the InfoOverlay
     **/
    class InfoProvider
    {
    public:
        InfoProvider(void);
        virtual ~InfoProvider(void);

    public:///Informators
        virtual const std::string Info() const = 0;
    };

}
#endif 

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='