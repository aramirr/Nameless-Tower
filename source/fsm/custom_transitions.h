#pragma once

#include "transition.h"
#include "utils/variant.h"

namespace FSM
{
  class VariableTransition : public ITransition
  {
    virtual bool checkCondition(CContext& ctx) const override;
    virtual bool load(const json& jData) override;

  private:
    enum class EOperation { EQUAL = 0, GREATER, GREATER_EQUAL };
    CVariant _variable;
    EOperation _operation = EOperation::EQUAL;
  };

	class LogicTransition : public ITransition
	{
		virtual bool checkCondition(CContext& ctx) const override;
		virtual bool load(const json& jData) override;

	private:
		enum class EOperation2 { EQUAL = 0, GREATER, GREATER_EQUAL };
		enum class ELogicOperation { AND = 0, OR };
		std::vector<CVariant> _variables;
		EOperation2 _operation = EOperation2::EQUAL;
		ELogicOperation _logic_operation = ELogicOperation::AND;
	};

  class TimeTransition : public ITransition
  {
    virtual bool checkCondition(CContext& ctx) const override;
    virtual bool load(const json& jData) override;

  private:
    float _time = 0.f;
  };
}
