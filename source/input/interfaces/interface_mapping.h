#pragma once

#include "input/button.h"

namespace Input
{
	class CHost;
	class CMapping;

	struct TInterface_Mapping
	{
		TInterface_Mapping(CHost& owner);
		void update(float delta);
		void clear();
		void assignMapping(const CMapping& mapping);

		const TButton& button(const std::string& name) const;

	private:
		struct TMappedButton
		{
			std::vector<const TButton*> buttons;
			bool all = false;
			TButton result;
		};
		using MButtons = std::map<std::string, TMappedButton>;

		CHost& _host;
		MButtons _buttons;
	};
}
