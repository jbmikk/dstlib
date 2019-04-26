#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "result.h"


DEFINE(_Result, unsigned int, UInt, uint)

void t_setup(){
}
void t_teardown(){
}

void result__result_ok(){
	Result(UInt) r = ResultOk(UInt, 70000);

	t_assert(r.result.type == _Result_TYPE_Ok);
	t_assert(r.data == 70000);

	SwitchType(r) {
	case ResultType(Ok):
		t_assert(1);
		break;
	case ResultType(Error):
		//Should never happen
		t_assert(0);
		break;
	}
}

void result__result_error(){
	Result(UInt) r = ResultError(UInt, -1);

	t_assert(r.result.type == _Result_TYPE_Error);
	t_assert(r.error == -1);

	SwitchType(r) {
	case ResultType(Ok):
		//Should never happen
		t_assert(0);
		break;
	case ResultType(Error):
		t_assert(1);
		break;
	}
}

int main(int argc, char** argv) {
	t_init();
	t_test(result__result_ok);
	t_test(result__result_error);
	return t_done();
}

