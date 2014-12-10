define([
	"appData"
],function(appData){

    var configFn = function($routeProvider){
    	
    	for(var k in appData.routers){
    		var router = appData.routers[k];

			$routeProvider.when(router.router, {
				templateUrl: '',//'scripts/modules/'+router.name+'/main.tpl', 
				controller: router.name+'Controller'
			});
    	}

        $routeProvider.otherwise(appData.routers[0].router);

        //$rootScope.$broadcast('$routeChangeSuccess', nextRoute, lastRoute);
    };
    return configFn;
});