#pragma once
#include <memory>
#include <pidmx_utils.h>
#include <vector>

class ICommand {
public:
	virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};
typedef std::shared_ptr<ICommand> CommandInstance;
