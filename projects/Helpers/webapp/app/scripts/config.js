define([
],function(angular,$,bs,app){

	if(!window.configData){
		
		//全局变量
		window.appConfig = true;
		window.mainAppPath = 'modules/main/';
		window.commonPath = 'modules/common/';


		//配置数据
		var configData = {};
		var routers = [{
			"name":"resUpdater",
			"title":"资源更新器",
			"router":"/resUpdater",
			"inTab":true
		},{
			"name":"about",
			"name":"关于",
			"router":"/about",
			"inTab":true
		}];
		configData.routers = routers;
		
		window.configData = configData;
	}
    
    return window.configData;
});