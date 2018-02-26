#pragma once

#include <vector>

namespace FSM
{
  class CContext;

  class IState
  {
  public:
    virtual void onStart(CContext& ctx) const {}
    virtual void onFinish(CContext& ctx) const {}
    virtual bool update(float dt, CContext& ctx) const { return _isFinal; }

    const std::string& getName() const { return _name; }
    bool isFinal() const { return _isFinal; }

  private:
    std::string _name;
    bool _isFinal = false;
  };

  using VStates = std::vector<IState*>;
}
