define([
	"appData"
],function(appData){

    var configFn = function($routeProvider){
    	
    	for(var k in appData.routers){
    		var router = appData.routers[k];
            var tplName = router.tpl?router.tpl:"main.tpl";
			$routeProvider.when(router.router, {
				//template: router.name,//'scripts/modules/'+router.name+'/main.tpl',
                templateUrl:"scripts/modules/"+router.name+"/"+tplName,
				controller: router.name+"Controller"
			});
    	}

        $routeProvider.otherwise(appData.routers[0].router);

        //$rootScope.$broadcast('$routeChangeSuccess', nextRoute, lastRoute);
    };
    return configFn;
});