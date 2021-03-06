  function GetFakkuImageURL: Boolean;
  var
    i, totalPages: Cardinal;
    l: TStringList;
    imgURL, imgExt, s: String;

  begin
    totalPages := 0;
    Task.Container.PageLinks.Clear;
    l := TStringList.Create;
    s := FillMangaSiteHost(FAKKU_ID, URL);
    Result := GetPage(TObject(l), s, Task.Container.Manager.retryConnect);

    if l.Count > 0 then
    begin
      //Get total pages
      for i := 0 to l.Count - 1 do
      begin
        if (Pos('window.params.thumbs = ', l[i]) > 0) then
        begin
          s := GetString(l[i], '= [', '];');
          with TStringList.Create do
            try
              DelimitedText := s;
              totalPages := Count;
            finally
              Free;
            end;
          Break;
        end;
      end;

      if totalPages = 0 then
        Result := False;

      //Get imgurl
      for i := 0 to l.Count - 1 do
      begin
        if (Pos('return ''', l[i]) > 0) then
        begin
          imgURL := GetString(l[i], 'return ''', ''' +');
          imgExt := GetString(l[i], 'x + ''', ''';');
          Break;
        end;
      end;

      //Build image links
      if imgExt = '' then
        imgExt := '.jpg';

      if imgURL <> '' then
      begin
        imgURL := TrimLeftChar(imgURL, ['/', '\', ':']);
        for i := 1 to totalPages do
        begin
          s := IntToStr(i);
          while Length(s) < 3 do
            s := '0' + s;
          Task.Container.PageLinks.Add(imgURL + s + imgExt);
        end;
      end;
    end;
    l.Free;
  end;
