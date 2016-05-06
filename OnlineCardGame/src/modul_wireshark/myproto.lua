-- create myproto protocol and its fields
p_myproto = Proto ("myproto","My Protocol")

tcp_src_f = Field.new("tcp.srcport")
tcp_dst_f = Field.new("tcp.dstport")

-- myproto dissector function
function p_myproto.dissector (buf, pkt, root)
  -- validate packet length is adequate, otherwise quit
  if buf:len() == 0 then return end
  pkt.cols.protocol = p_myproto.name
 
  -- create subtree for myproto
  local subtree = root:add(p_myproto, buf(0))

    if (tcp_dst_f().value == 8888) then

        local option = buf(0,1):uint()

        subtree:add(buf(0,1), "Option: " .. buf(0,1):uint())
        subtree:add(buf(1,1), "Category: " .. buf(1,1):uint())

        if (option == 3) then
            subtree:add(buf(2,buf:len()-3), "Advert: " .. buf(2,buf:len()-3):string())
        else
            if (option == 4) then
                subtree:add(buf(2,1), "Created ago: " .. buf(2,1):uint())
            end
        end

    else
        subtree:add(buf(0,1), "Category: " .. buf(0,1):uint())
        subtree:add(buf(1,8), "Created: " .. buf(1,8):le_int64())
        subtree:add(buf(9,buf:len()-10), "Advert: " .. buf(9,buf:len()-10):string())
    end

end

-- Initialization routine
function p_myproto.init()
end

-- register a chained dissector for port 8888
local tcp_dissector_table = DissectorTable.get("tcp.port")
dissector = tcp_dissector_table:get_dissector(8888)
tcp_dissector_table:add(8888, p_myproto)
