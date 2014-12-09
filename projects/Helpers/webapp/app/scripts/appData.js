define([
],function(angular,$,bs,app){

	if(!window.configData){
	
		//配置数据
		var configData = {};
		var routers = [{
			"name":"resUpdater",
			"title":"资源更新器",
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
	}
    
    return window.configData;
});