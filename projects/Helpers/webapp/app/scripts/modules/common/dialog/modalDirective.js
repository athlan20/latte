define([
    'jquery',
    commonPath+'config'

],function($,common,modalTpl){
    var directive = ['$rootScope','$http',function($rootScope,$http){
      return{

        scope: true,
        link:function(scope,element,attrs){
            scope.btnText1 = "关闭";
            scope.btnText2 = "";
        }

      }
    }]

    
    return directive;
});