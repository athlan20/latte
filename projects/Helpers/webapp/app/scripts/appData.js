define([
],function(angular,$,bs,app){

	if(!window.configData){
	
		//配置数据
		var configData = {};
		var routers = [{
			"name":"resUpdater",
			"title":"资源打包器",
			"router":"/resUpdater",
			"inTab":true
		},{
			"name":"about",
			"title":"关于",
			"router":"/about",
			"inTab":true
		}];
		configData.routers = routers;
		
		window.configData = configData;

		window.getRouteObj = function(name){
			var routeObj = null;
			for(var k in routers){
				routeObj = routers[k];
				if(routeObj.name==name){
					return routeObj;
				}
			}
			return routeObj;
		};
	}
    
    return window.configData;
});