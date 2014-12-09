
define([
    'jquery',
    'angular',
    'angular-route',
    'route',
    commonPath+'dialog/modalDialog'

],function($,angular,ngRoute,route,__){
    var routeResolver = function () {

        this.$get = function () {
            return this;
        };

        this.routeConfig = function () {
            var viewsDirectory = '/app/views/',
                controllersDirectory = '/app/controllers/',

            setBaseDirectories = function (viewsDir, controllersDir) {
                viewsDirectory = viewsDir;
                controllersDirectory = controllersDir;
            },

            getViewsDirectory = function () {
                return viewsDirectory;
            },

            getControllersDirectory = function () {
                return controllersDirectory;
            };

            return {
                setBaseDirectories: setBaseDirectories,
                getControllersDirectory: getControllersDirectory,
                getViewsDirectory: getViewsDirectory
            };
        }();

        this.route = function (routeConfig) {

            var resolve = function (baseName, path, secure) {
                if (!path) path = '';

                var routeDef = {};
                routeDef.templateUrl = routeConfig.getViewsDirectory() + path + baseName + '.html';
                routeDef.controller = baseName + 'Controller';
                routeDef.secure = (secure) ? secure : false;
                routeDef.resolve = {
                    load: ['$q', '$rootScope', function ($q, $rootScope) {
                        var dependencies = [routeConfig.getControllersDirectory() + path + baseName + 'Controller.js'];
                        return resolveDependencies($q, $rootScope, dependencies);
                    }]
                };

                return routeDef;
            },

            resolveDependencies = function ($q, $rootScope, dependencies) {
                var defer = $q.defer();
                require(dependencies, function () {
                    defer.resolve();
                    $rootScope.$apply()
                });

                return defer.promise;
            };

            return {
                resolve: resolve
            }
        }(this.routeConfig);

    };

    var servicesApp = angular.module('routeResolverServices', []);

    //Must be a provider since it will be injected into module.config()    
    servicesApp.provider('routeResolver', routeResolver);

    var app = angular.module("mainApp",["ngRoute","routeResolverServices","mainApp.common.modalDialog"]);

    app.config(["routeResolverServices",function(routeResolverServices){}]);

    app.run(['$rootScope',function($rootScope){

        // $rootScope.dir = BlogInfo.url;
        // $rootScope.site = BlogInfo.site;
        // $rootScope.api = AppAPI.url;
        
        setTimeout(function(){
            //$rootScope.$emit("showModal");
        },1000);        

    }]);



    
    
    return app;
});