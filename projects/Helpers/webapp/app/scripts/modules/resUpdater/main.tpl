<resUpdater class="input-group input-group-md" style="margin:20% auto;" ng-controller="resUpdaterController">
	<div class="container">
		<div class="row">
			<div class="input-group">
				<span class="input-group-addon">源路径</span>
				<input type="text" class="form-control" placeholder="打包源文件夹" value="{{packageSrcPath}}">
				<span class="input-group-btn">
					<button class="btn btn-default" type="button" ng-click="selectSrcDirectory()">浏览</button>
  				</span>
			</div>
		</div>
		<div style="margin-top:20px"></div>
		<div class="row">
			<div class="input-group">
				<span class="input-group-addon">目的地</span>
				<input type="text" class="form-control" placeholder="打包目标文件夹" value="{{packageDestPath}}">
				<span class="input-group-btn">
					<button class="btn btn-default" type="button" ng-click="selectDestDirectory()">浏览</button>
  				</span>
			</div>			
		</div>
		<div style="margin-top:20px"></div>
		<div class="row">
			<div class="input-group">
				<span class="input-group-addon">版本号</span>
				<input type="text" class="form-control" placeholder="版本号" value="{{packageVersion}}">
				<span class="input-group-addon">
					0.001
  				</span>
			</div>			
		</div>
		<div style="margin-top:20px"></div>
		<div class="row text-center">
			<button type="button" class="btn btn-default btn-ms" ng-click="submitPackage()">
				<span class="glyphicon glyphicon-star" aria-hidden="true"></span> 打包
			</button>
		</div>
	</div>
</resUpdater>