#ifndef WALI_UTIL_CONFIGURATION_VAR_HPP
#define WALI_UTIL_CONFIGURATION_VAR_HPP

#include <cstdlib>
#include <string>
#include <map>
#include <boost/any.hpp>

namespace wali
{
  namespace util 
  {
    template<typename BackingType>
    class ConfigurationVar
    {
      std::map<std::string, BackingType> options_;
      BackingType default_;
      std::string env_var_name_;

    public:
      ConfigurationVar(std::string env_var_name,
                            BackingType const & default_val)
        : default_(default_val)
        , env_var_name_(env_var_name)
      {}

      void
      setDefault(BackingType const & val)
      {
        default_ = val;
      }

      ConfigurationVar &
      operator() (std::string const & name, BackingType const & val)
      {
        assert(options_.count(name) == 0u);
        options_[name] = val;
        return *this;
      }

      BackingType const &
      getDefault() const
      {
        return default_;
      }

      std::string const &
      getEnvVarName() const
      {
        return env_var_name_;
      }

      BackingType const &
      getValueOf(std::string const & name) const
      {
        typename std::map<std::string, BackingType>::const_iterator
          iter = options_.find(name);
        if(iter != options_.end()) {
          // TODO print warning
          return iter->second;
        }
        else {
          return getDefault();
        }
      }

      operator BackingType() const
      {
        std::string env_var_name = getEnvVarName();
        char * env_var_value = std::getenv(env_var_name.c_str());
        if (env_var_value != NULL) {
          return getValueOf(env_var_value);
        }
        else {
          return getDefault();
        }
      }


    };

  }
}

#endif
