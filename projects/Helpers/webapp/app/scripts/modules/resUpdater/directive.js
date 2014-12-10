define([
    'jquery'
],function($){
    var directive = ['$rootScope','$scope','$http',function($rootScope,$scope,$http){
        return{

            scope: true,
            link:function(scope,element,attrs){
                scope.selectDirectory=function(){
                    alert('selectDirectory')
                }
            }

        }
    }];
    return directive;
});