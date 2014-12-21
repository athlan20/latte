var cefQuery = window.cefQuery;
function nativeCall(funcName,funcParam,successCall,failCall){
	var requestObj = {};
	requestObj.funcName =  funcName;
	requestObj.funcParam = funcParam?funcParam:"";
	var requestStr = JSON.stringify(requestObj);

	var callObj = {};
	callObj.request = requestStr;
	callObj.persistent = false;
	callObj.onSuccess = function(response){
		if(successCall){
			successCall(response);
		}
	};
	callObj.onFailure = function(error_code, error_message){
		if(failCall){
			failCall();
		}
	};
	
	var queryId = cefQuery(callObj);
	// window.cefQueryCancel(queryId);
}
function nativeCallJs(param){
	if(rootScope){
		if(isDebug){
			//console.log(param)
		}
		var call = JSON.parse(param);
		if(!call.hasOwnProperty("param"))
			call.param = "";
		rootScope.$emit(call.funcName,call.param)
	}else{
		console.log("error no rootScope");
	}
}