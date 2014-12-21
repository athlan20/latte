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
                scope.updateSrcPath = "d:\\Users\\l_xi\\Desktop\\mine";
                scope.nowLoaded = 0;
                scope.totalLoaded = 1;
                scope.loadedNum = 0;
                scope.totalNum = 1;
                scope.loadingPercent = 0;
                scope.loadedPercent = 0;

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

                //native开始加载
                $rootScope.$on('startLoad',function(target,param){
                    scope.nowLoaded = 0;
                    scope.totalLoaded = 100;
                    scope.loadedNum = 0;
                    scope.totalNum = param.totalNum;

                    scope.loadingPercent = 0;
                    scope.loadedPercent = 0;

                    scope.$digest();
                })
                $rootScope.$on('onLoading',function(target,param){
                    scope.nowLoaded = param.nowLoaded;
                    scope.totalLoaded = param.totalLoaded;
                    var percent = (scope.nowLoaded / scope.totalLoaded *100)>>0;
                    if(percent != scope.loadingPercent)
                    {
                        scope.loadingPercent = percent;
                        scope.$digest();
                    }
                    
                })
                $rootScope.$on('onLoaded',function(target,param){
                    scope.nowLoaded = 0;
                    scope.totalLoaded = 100;
                    scope.loadedNum = scope.loadedNum+1;

                    scope.loadingPercent = 100;
                    scope.loadedPercent = (scope.loadedNum/scope.totalNum*100)>>0;
                    
                    scope.$digest();
                })
            }

        }
    }];
    return directive;
});