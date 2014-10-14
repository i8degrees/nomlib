function testme(event, element, document)
  r = 'Hello, world!';
  print(r);
  print("element ID: " .. element.id);
  print("parent element ID: " .. element.parent_node.id);
  print("event type: " .. event.type);
  print("mouse_x: " .. event.parameters["mouse_x"]);
  print("mouse_y: " .. event.parameters["mouse_y"]);

  -- FIXME: Never received by C++ interface =(
  params = { res = "bingo!" };
  document:GetElementById("p").DispatchEvent( "testme", params, false );
end
