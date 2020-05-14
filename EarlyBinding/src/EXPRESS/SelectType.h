/*
	This file is part of Expresso, a simple early binding generator for EXPRESS.
	Copyright (c) 2016 Technical University of Munich
	Chair of Computational Modeling and Simulation.

	BlueFramework is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License Version 3
	as published by the Free Software Foundation.

	BlueFramework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once
#ifndef OpenInfraPlatform_EarlyBinding_SelectType_10255db4_8b09_413b_9c43_ea39f1d63ced_h
#define OpenInfraPlatform_EarlyBinding_SelectType_10255db4_8b09_413b_9c43_ea39f1d63ced_h

#include "../EarlyBinding/src/namespace.h"

#include "ValueType.h"
#include "EXPRESSModel.h"
//#include "EXPRESSEntity.h"

#include <type_traits>

#include <boost/variant.hpp>
#include <boost/preprocessor.hpp>

#include <tuple>
#include <utility>

OIP_NAMESPACE_OPENINFRAPLATFORM_EARLYBINDING_BEGIN


class visitor_getStepParameter
	: public boost::static_visitor<std::string>
{
public:
	template <typename T>
	const std::string operator()(const T& operand) const
	{
		return operand.classname() + "(" + operand.getStepParameter() + ")";
	}
};



template <typename ...Args> class SelectType : public ValueType<boost::variant<Args...>> {
	using base = ValueType<boost::variant<Args...>>;

	template <typename TCast> class visitor_cast : public boost::static_visitor<TCast> {
		public:
		template <typename TVisited>
		TCast operator()(const TVisited &operand) const {
			if(boost::detail::variant::holds_element<decltype(operand.m_value), TCast >::value)
				return boost::get<TCast>(operand.m_value);
			else
				return (TCast) operand;
		}
	};

	friend class visitor_cast<double>;

	template<std::size_t I = 0, typename Function>
	static inline typename std::enable_if<I == sizeof...(Args), void>::type
		for_each(std::tuple<Args...>, Function) // Unused arguments are given no names.
	{ };

	template<std::size_t I = 0, typename Function>
	static inline typename std::enable_if < I < sizeof...(Args), void>::type
		for_each(std::tuple<Args...> t, Function f)
	{
		f(std::get<I>(t));
		for_each<I + 1, Function>(t, f);
	};

public:
	typedef boost::variant<Args...> Select;

	using base::base;
	using base::operator=;
	using base::operator->;

	const size_t which() const { return base::m_value.which(); };

	template <class T> explicit operator T&() & {
		//static_assert(boost::detail::variant::holds_element<Select, T >::value, "Cast to type is not defined.");
		//return std::ref(boost::get<T>(base::m_value));
		return std::ref(boost::apply_visitor(visitor_cast<T>(), base::m_value));
	}
	template <class T> explicit operator const T() const {
		//static_assert(boost::detail::variant::holds_element<Select, T >::value, "Cast to type is not defined.");
		//return boost::get<T>(base::m_value);
		return boost::apply_visitor(visitor_cast<T>(), base::m_value);
	}

	template <size_t Index> auto get() -> std::tuple_element_t<Index, std::tuple<Args...>> {
		return boost::get<std::tuple_element_t<Index, std::tuple<Args...>>>(base::m_value);
	}

	template <class T> T get() {
		static_assert(boost::detail::variant::holds_element<Select, T >::value, "Cast to type is not defined.");
		return boost::get<T>(base::m_value);
	}

	virtual const std::string getStepParameter() const {
		return boost::apply_visitor(visitor_getStepParameter(), base::m_value);
	}

	static Select readStepData(const std::string value, const std::shared_ptr<EXPRESSModel>& model) {
		Select select;
		std::tuple<Args...> variadicArgs = std::tuple<Args...>();
		if (value == "*") {
			//TODO
		} else {
			if (value[0] == '#') {
				size_t refId = std::stoull(value.substr(1, value.size() - 1));
				if (model->entities.count(refId) > 0) {
					auto refEntity = model->entities[refId];
					//TODO
					SelectType::for_each(variadicArgs, [&select, &refEntity, &value, &model](auto type) {
						typedef typename std::conditional<std::is_base_of<EXPRESSType, decltype(type)>::value, EXPRESSEntity, typename decltype(type)::element_type>::type CastType;
						if (!std::is_base_of<EXPRESSType, decltype(type)>::value && std::dynamic_pointer_cast<CastType>(refEntity)) {
							type = decltype(type)::readStepData(value, model);
							select = type;
						}
					});
				}
				else {
					//TODO:: error
					throw std::exception("Entity not contained in model.");
				}
			}
			else {
				auto name = value.substr(0, value.find_first_of('('));
				auto startpos = value.find_first_of('(') + 1;
				auto arg = value.substr(startpos, value.find_last_of(')') - startpos);

				SelectType::for_each(variadicArgs, [&select, &name, &arg, &model](auto type) {
					if (std::is_base_of<EXPRESSType, decltype(type)>::value && name == type.classname()) {
						type = decltype(type)::readStepData(arg, model);
						select = type;
					}
				});
			}
		}
		return select;
	}
};

OIP_NAMESPACE_OPENINFRAPLATFORM_EARLYBINDING_END

#endif // end define OpenInfraPlatform_EarlyBinding_EXPRESSSelect_10255db4_8b09_413b_9c43_ea39f1d63ced_h
