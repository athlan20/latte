define([
    'jquery'
],function($,mainTpl){
    var directive = ['$rootScope','$http',function($rootScope,$http){
        return{
            restrict: "E",
            scope: true,
            link:function(scope,element,attrs){
                scope.selectDirectory=function(){
                    nativeCall("selectDirectory",null,function(res){
                        console.log(res)
                    });
                    //window.cefQuery({request:"abc"});
                }
            }

        }
    }];
    return directive;
});