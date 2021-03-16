// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#include <noggit/Log.h>
#include <noggit/scripting/script_loader.hpp>
#include <noggit/scripting/script_filesystem.hpp>
#include <noggit/scripting/scripting_tool.hpp>

#include <sol/sol.hpp>
#include <noggit/scripting/script_loader-noggit_module.ipp>

static bool initialized = false;
static int cur_script = -1;

using noggit::scripting::get_cur_tool;

struct script_container
{
  script_container(
      std::string name,
      std::string display_name,
      std::function<void()> left_click) :
                            _name(name),
                            _display_name(display_name),
                            _on_left_click(left_click)
  {
  }
  script_container() = default;

  std::string _name;
  std::string _display_name;
  std::function<void()> _on_left_click;
};

static std::vector<script_container> containers;

static script_container *get_container()
{
  if (cur_script < 0)
  {
    return nullptr;
  }
  return &containers[cur_script];
}

static bool ends_with(std::string const &str, std::string const &suffix)
{
  return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

std::function<void()> global_cb;

static void register_script(std::string name, std::function<void()> cb)
{
  global_cb = cb;
}

static void print(std::string value)
{
  get_cur_tool()->addLog(value);
}

namespace noggit
{
  namespace scripting
  {
    int get_selected_script()
    {
      return cur_script;
    }

    std::string selected_script_name()
    {
      return cur_script >= 0 && cur_script < containers.size()
                 ? containers[cur_script]._name
                 : "";
    }

    std::string const &get_script_name(int id)
    {
      return containers[id]._name;
    }

    std::string const &get_script_display_name(int id)
    {
      return containers[id]._display_name;
    }

    int script_count()
    {
      return containers.size();
    }

    void send_left_click(){}
    void send_left_hold() { if (global_cb) global_cb(); }
    void send_left_release(){}

    void send_right_click(){}
    void send_right_hold(){}
    void send_right_release(){}

    sol::state * lua;

    int load_scripts()
    {
      std::string old_module = cur_script > 0 ? get_script_name(cur_script) : "";
      cur_script = -1;
      int new_index = -1;

      // TODO: leak
      lua = new sol::state();

      lua->set_function("register_script",register_script);
      lua->set_function("print",print);
      register_functions(lua);

      containers.clear();

      boost::filesystem::recursive_directory_iterator dir("scripts"), end;

      while (dir != end)
      {
        if (boost::filesystem::is_directory(dir->path()))
          continue;
        std::string file = dir->path().string();
        ++dir;
        if (!ends_with(file, ".lua") || ends_with(file, ".spec.lua"))
          continue;

        get_cur_tool()->addLog(file);

        try {
          lua->script_file(file);
        }
        catch (std::exception e) {
          get_cur_tool()->addLog("File error!");
        }

        if (false)
        {
          get_cur_tool()->setStyleSheet("background-color: #f0a5a5;");
          get_cur_tool()->addLog("Script error:");

          /*
          for (auto &err : program->errors)
          {
            get_cur_tool()->addLog(reportError(err.at, err.what, err.extra, err.fixme, err.cerr));
          }
          */
          get_cur_tool()->resetLogScroll();
          containers.clear();
          return -1;
        }

        bool is_any = false;

        if (is_any)
        {
          auto module_name = file.substr(8, file.size() - 12);
          if (module_name == old_module)
          {
            new_index = containers.size();
          }

          auto display_name = module_name;
          auto title_fun = "";

          containers.push_back(script_container(
              module_name,
              display_name,
              nullptr));
        }
      }

      if (new_index == -1 && containers.size() > 0)
      {
        new_index = 0;
      }

      // remove error color on successful compile
      get_cur_tool()->setStyleSheet("");

      return new_index;
    }

    void select_script(int index)
    {
      // just for safety
      save_json();

      if (index < 0 || index >= containers.size())
      {
        return;
      }

      cur_script = index;
      auto ref = &containers[index];
      try
      {
        //CALL_FUNC(ref, select);
      }
      catch (std::exception const &e)
      {
        get_cur_tool()->addLog(("[error]: " + std::string(e.what())));
        get_cur_tool()->resetLogScroll();
      }
    }
  } // namespace scripting
} // namespace noggit