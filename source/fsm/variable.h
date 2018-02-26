#pragma once

#include <vector>

namespace FSM
{
  class CVariable
  {
  public:
    enum class EType { BOOL = 0, INT, FLOAT, STRING };

    const std::string& getName() const { return _name; }
    EType getType() const { return _type; }

    bool getBool() const { return _bValue; }
    int getInt() const { return _iValue; }
    float getFloat() const { return _fValue; }
    const std::string& getString() const { return _sValue; }

    void setBool(bool value) { _bValue = value; }
    void setInt(int value) { _iValue = value; }
    void setFloat(float value) { _fValue = value; }
    void setString(const std::string& value) { _sValue = value; }

  private:
    std::string _name;
    EType _type = EType::BOOL;
    union
    {
      bool _bValue;
      int _iValue;
      float _fValue;
      std::string _sValue;
    };
  };

  // example of alternative implementation:
  //class CBoolVariable : public CVariable
  //{
  //public:
  //  bool getBoolValue() override { return _value; }

  //private:
  //  bool value;
  //};

  using VVariables = std::vector<CVariable>;
}
