define([
    'jquery'
],function($,mainTpl){
    var directive = ['$rootScope','$http',function($rootScope,$http){
        return{
            restrict: "E",
            scope: true,
            link:function(scope,element,attrs){
                scope.selectDirectory=function(){
                    nativeCall("selectDirectory",null);
                }
            }

        }
    }];
    return directive;
});