#pragma once

#include <memory>
#include "command_result.h"
#include "command_result_factory.h"
#include "sdx_optional.h"
#include <string>
#include <vector>

namespace Sdx
{
  namespace Cmd
  {
    ///
    /// Result of GetNavICEphDoubleParamForEachSV.
    ///
    /// Name        Type            Description
    /// ----------- --------------- --------------------------------------------------------------------------------------------------
    /// ParamName   string          Refer to SetNavICEphDoubleParamForSV for accepted names
    /// Val         array double    Parameter value for each satellite. Zero based index (index 0 => SV ID 1, index 1 => SV ID 2, etc)
    /// DataSetName optional string Optional name of the data set to use. If no value is provided, the active data set is used.
    ///

    class GetNavICEphDoubleParamForEachSVResult;
    typedef std::shared_ptr<GetNavICEphDoubleParamForEachSVResult> GetNavICEphDoubleParamForEachSVResultPtr;
    typedef GetNavICEphDoubleParamForEachSVResult GetNavICEphemerisDoubleParamsResult;
    typedef std::shared_ptr<GetNavICEphemerisDoubleParamsResult> GetNavICEphemerisDoubleParamsResultPtr;
    
    class GetNavICEphDoubleParamForEachSVResult : public CommandResult
    {
    public:
      static const char* const CmdName;
      static const char* const Documentation;


      GetNavICEphDoubleParamForEachSVResult();

      GetNavICEphDoubleParamForEachSVResult(const std::string& paramName, const std::vector<double>& val, const Sdx::optional<std::string>& dataSetName = {});

      GetNavICEphDoubleParamForEachSVResult(CommandBasePtr relatedCommand, const std::string& paramName, const std::vector<double>& val, const Sdx::optional<std::string>& dataSetName = {});

      static GetNavICEphDoubleParamForEachSVResultPtr create(const std::string& paramName, const std::vector<double>& val, const Sdx::optional<std::string>& dataSetName = {});

      static GetNavICEphDoubleParamForEachSVResultPtr create(CommandBasePtr relatedCommand, const std::string& paramName, const std::vector<double>& val, const Sdx::optional<std::string>& dataSetName = {});
      static GetNavICEphDoubleParamForEachSVResultPtr dynamicCast(CommandBasePtr ptr);
      virtual bool isValid() const override;
      virtual std::string documentation() const override;


      // **** paramName ****
      std::string paramName() const;
      void setParamName(const std::string& paramName);


      // **** val ****
      std::vector<double> val() const;
      void setVal(const std::vector<double>& val);


      // **** dataSetName ****
      Sdx::optional<std::string> dataSetName() const;
      void setDataSetName(const Sdx::optional<std::string>& dataSetName);
    };
    REGISTER_COMMAND_RESULT_TO_FACTORY_DECL(GetNavICEphDoubleParamForEachSVResult);
  }
}

