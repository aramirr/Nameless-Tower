#pragma once

#include <vector>
#include "components/juan/comp_transform.h"
#include "components/camera/comp_camera.h"
#include "components/ui/ui_mouse_pos.h"

namespace FSM
{
  class CContext;

  class IState
  {		
  public:
		DECL_SIBLING_ACCESS();
    virtual void onStart(CContext& ctx) const {}
    virtual void onFinish(CContext& ctx) const {}
    virtual bool update(float dt, CContext& ctx) const { return _isFinal; }
    virtual bool load(const json& jData) { (void)jData; return false; }

    void setName(const std::string& name) { _name = name; }
    const std::string& getName() const { return _name; }
    void setFinal(bool how) { _isFinal = how; }
    bool isFinal() const { return _isFinal; }

  private:
    std::string _name;
    bool _isFinal = false;
  };

  using VStates = std::vector<IState*>;
}
