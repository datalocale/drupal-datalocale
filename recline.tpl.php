<div class="recline-app">
  

  <div class="container-fluid">
    <div class="content">
      <div class="page-home backbone-page"></div>
    </div>

      <div class="page-explorer backbone-page">
        <div class="data-explorer-here"></div>
      </div>
    </div>

  <!-- modals for menus -->
  <div class="modal fade in js-load-dialog-url" style="display: none;">
    <div class="modal-header">
      <a class="close" data-dismiss="modal">×</a>
      <h3>Load from URL</h3>
    </div>
    <div class="modal-body">
      <form class="js-load-url">
        <div class="control-group">
          <div class="controls">
            <input type="text" name="source" class="span5" placeholder="URL to data source" />
            <p class="help-block"></p>
            <input name="backend_type" style="display: none;" />
          </div>
        </div>
        <button type="submit" class="btn btn-primary">Load &raquo;</button>
      </form>
    </div>
  </div>

  <div class="modal fade in js-load-dialog-file" style="display: none;">
    <div class="modal-header">
      <a class="close" data-dismiss="modal">×</a>
      <h3>Load from File</h3>
    </div>
    <div class="modal-body">
      <form class="form-horizontal">
        <div class="control-group">
          <label class="control-label">File</label>
          <div class="controls">
            <input type="file" name="source" />
          </div>
        </div>
        <div class="control-group">
          <label class="control-label">Separator</label>
          <div class="controls">
            <input type="text" name="separator" value="," class="spam1"/>
          </div>
        </div>
        <div class="control-group">
          <label class="control-label">Text delimiter</label>
          <div class="controls">
            <input type="text" name="delimiter" value='"' />
          </div>
        </div>

        <div class="control-group">
          <label class="control-label">Encoding</label>
          <div class="controls">
            <input type="text" name="encoding" value="UTF-8" />
          </div>
        </div>
        <div class="form-actions">
          <button type="submit" class="btn btn-primary">Load &raquo;</button>
        </div>
      </form>
    </div>
  </div>

  <div class="modal fade in js-share-and-embed-dialog" style="display: none;">
    <div class="modal-header">
      <a class="close" data-dismiss="modal">×</a>
      <h3>Share and Embed</h3>
    </div>
    <div class="modal-body">
      <h4>Sharable Link to current View</h4>
      <textarea class="view-link" style="width: 100%; height: 100px;"></textarea>
      <h4>Embed this View</h4>
      <textarea class="view-embed" style="width: 100%; height: 200px;"></textarea>
    </div>
  </div>

  <div class="modal fade in js-settings" style="display: none;">
    <div class="modal-header">
      <a class="close" data-dismiss="modal">×</a>
      <h3>Settings</h3>
    </div>
    <div class="modal-body">
      <form class="form-horizontal">
        <div class="control-group">
          <label class="control-label">DataHub API Key</label>
          <div class="controls">
            <input type="text" name="datahub_api_key" value="" />
            <p class="help-block"><strong>Getting your API key:</strong> Register/Login to <a href="http://datahub.io/">http://datahub.io/</a> and then visit your user home page (click on the link at the top right). On your home page your API key is located at the top of the page in the section showing your main user details.</p>
          </div>
        </div>
        <div class="form-actions">
          <button type="submit" class="btn btn-primary">Save &raquo;</button>
        </div>
      </form>
    </div>
  </div>
</div>