/*
    Copyright (c) 2018 Technical University of Munich
    Chair of Computational Modeling and Simulation.

    TUM Open Infra Platform is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    TUM Open Infra Platform is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef OpenInfraPlatform_EarlyBinding_namespace_49ee5b2c_7ffa_40d7_8382_c27a2812796b_h
#define OpenInfraPlatform_EarlyBinding_namespace_49ee5b2c_7ffa_40d7_8382_c27a2812796b_h
#if !defined OIP_NAMESPACE_OPENINFRAPLATFORM_EARLYBINDING_BEGIN
#define OIP_NAMESPACE_OPENINFRAPLATFORM_EARLYBINDING_BEGIN \
namespace OpenInfraPlatform { \
namespace EarlyBinding {
#endif
#if !defined OIP_NAMESPACE_OPENINFRAPLATFORM_EARLYBINDING_END
#define OIP_NAMESPACE_OPENINFRAPLATFORM_EARLYBINDING_END \
}\
}
#endif
#if !defined EMBED_INTO_OIP_NAMESPACE
#define EMBED_INTO_OIP_NAMESPACE(X) \
namespace oip { \
using OpenInfraPlatform::EarlyBinding::X; \
}
#endif
#endif // end define OpenInfraPlatform_EarlyBinding_namespace_49ee5b2c_7ffa_40d7_8382_c27a2812796b_h