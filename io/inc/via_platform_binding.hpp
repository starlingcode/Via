/**
 * \file via_platform_binding.hpp
 * \brief Link the correct module class definition per the build target.
 *
 *  Include the correct module class definition per the preprocessor defines.
 *  The module firmware is built with BUILD_F373, the Rack plugin uses BUILD_VIRTUAL.
 *  See via_f373_module.hpp for hardware implementation details and via_virtual_module.hpp for the virtual implementation.
 */

#ifndef INC_HARDRWARE_IO_H_
#define INC_HARDRWARE_IO_H_

#ifdef BUILD_F373
#include "via_f373_module.hpp"
#endif

#ifdef BUILD_VIRTUAL
#include "via_virtual_module.hpp"
#endif


#endif /* INC_HARDRWARE_IO_H_ */
