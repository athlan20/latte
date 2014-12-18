define([
    'jquery'
],function($,mainTpl){
    var directive = ['$rootScope','$http',function($rootScope,$http){
        return{
            restrict: "E",
            scope: true,
            link:function(scope,element,attrs){
                var dirSelectType = 0;
                scope.packageVersion="";
                scope.selectSrcDirectory=function(){
                    dirSelectType=0;
                    nativeCall("selectDirectory");
                };
                scope.selectDestDirectory = function(){
                    dirSelectType=1;
                    nativeCall("selectDirectory");
                };
                scope.submitPackage = function(){
                    nativeCall("submitPackage",{srcPath:scope.packageSrcPath,destPath:scope.packageDestPath,version:scope.packageVersion})
                };
                scope.selectUpdateDirectory = function(){
                    dirSelectType=2;
                    nativeCall("selectDirectory");
                };
                scope.update = function(){
                    nativeCall("update",{updateSrcPath:scope.updateSrcPath})
                };

                $rootScope.$on('selectDirectory',function(target,param){
                    if(dirSelectType==0){
                        scope.packageSrcPath = param.path;
                    }else if(dirSelectType==1){
                        scope.packageDestPath = param.path;
                    }else{
                        scope.updateSrcPath = param.path;
                    }
                    scope.$digest();
                })
                $rootScope.$on("submitPackage",function(target,param){

                })
            }

        }
    }];
    return directive;
});