define([
	"config"
],function(configData){

    var configFn = function($routeProvider,routeResolverServices){
    	var route = routeResolverProvider.route;
    	
    	for(var k in configData.routers){
    		var router = configData.routers[k];
			$routeProvider.when(router.router, {
				templateUrl: 'scripts/modules/'+router.name+'/main.tpl', 
				// controller: router.name+'Controller'
				resolve: resolveController('/app/controllers/customersController.js')
			});
    	}
    };
    return configFn;
});