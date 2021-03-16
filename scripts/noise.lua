-- Simple noise test
register_script("func",function(reg)
	local algo = "HgANAAUAAAAAAABAEAAAAAA/CAAAAACAPwAAAAA/AAAAAAABEwBI4RpAGwANAAMAAAAAAABACAAAAAAAPwAAAAAAAI/C9Tw="
	local frequency = 0.05
	local amp = 40.0
	local sel = select_origin(pos(),outer_radius(),outer_radius())
	local map = make_noise_selection(sel,frequency,10,algo,"someseed")
	while sel_next_chunk(sel) do
		local chunk = sel_get_chunk(sel)
		while chunk_next_vert(chunk) do
			local vert = chunk_get_vert(chunk)
			local loc = vert_get_pos(vert)
			local height = noise_get(map,loc)*amp;
			vert_set_height(vert,height);
			vert_set_alpha(vert,1,0.5)
		end
		chunk_apply_all(chunk);
	end
end)