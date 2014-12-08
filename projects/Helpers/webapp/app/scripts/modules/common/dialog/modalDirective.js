define([
    'jquery',
    commonPath+'config'

],function($,common,modalTpl){
    var directive = ['$rootScope','$http',function($rootScope,$http){
      return{

        scope: true,
        link:function(scope,element,attrs){

        }

      }
    }]

    
    return directive;
});