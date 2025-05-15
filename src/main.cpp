
#include "./stuff.h"

#define MARGIN 30
const int width = 1100;
const int hight = 650;

bool isnumber(char *string)
{
    for (size_t i = 0; string[i] != '\0'; i++)
    {
        if (!isdigit(string[i]))
            return false;
    }
    return true;
}

bool preFix(std::string line, std::string text)
{
    for (int i = 0; i < text.size(); i++)
    {
        if (line[i] != text[i])
            return false;
    }
    return true;
}

struct process
{
    int pid;
    std::string name;
    int ppid;
    char state;
    int Uid;

    void killProcess()
    {
    }
    void setState(char newState)
    {
    }
};

class processHandler
{
public:
    std::vector<process> allPrsocesses;

    processHandler()
    {
        readData();
        //sortProcesses();
    }

    void readData()
    {
        allPrsocesses.clear();
        for (const auto &entry : std::filesystem::directory_iterator("/proc"))
        {
            if (entry.is_directory())
            {
                std::string name = entry.path().filename();
                if (isnumber((char *)name.c_str()))
                {
                    addProcess(getProcess(name));
                }
            }
        }
    }

    process getProcess(std::string id)
    {
        process result;

        std::ifstream proc("/proc/" + id + "/status");

        result.pid = std::stoi(id);

        std::string line;

        while (std::getline(proc, line))
        {
            if (preFix(line, "Name:"))
                result.name = line.substr(6);
            else if (preFix(line, "PPid:"))
                result.ppid = std::stoi(line.substr(6));
            else if (preFix(line, "State:"))
                result.state = line.substr(7, 1)[0];
            else if(preFix(line , "Uid:"))
                result.Uid =  std::stoi(line.substr(5 , 4));
        }

        return result;
    }
    void sortProcesses()
    {
        // for (int i = 0; i < allPrsocesses.size(); i++)
        // {
        //     for (int j = i + 1; j < allPrsocesses.size(); j++)
        //     {
        //         if (allPrsocesses[i].name > allPrsocesses[j].name)
        //         {
        //             process temp = allPrsocesses[i];
        //             allPrsocesses[i] = allPrsocesses[j];
        //             allPrsocesses[j] = temp;
        //         }
                
        //     }
        // }
        
    }
    void addProcess(process proc)
    {
        allPrsocesses.push_back(proc);
    }
    void print()
    {
        // for (int i = 0; i < allPrsocesses.size(); i++)
        // {
        //     // std::cout << "Process name : " << allPrsocesses[i].name << '\n';
        //     // std::cout << "Process pid : " << allPrsocesses[i].pid << '\n';
        //     // std::cout << "Process ppid : " << allPrsocesses[i].ppid << '\n';
        //     // std::cout << "----------------------------------\n";
        // }
        
    }
    ~processHandler()
    {
    }
};

// void test()
// {
//     lf_div_begin(((vec2){MARGIN , MARGIN}) , ((vec2){280 , hight - MARGIN * 2}) , true);
// }

bool substring(std::string input)
{
    
    std::string ddd = inputSearchProcessesString;
    

    if (input.find(ddd) != std::string::npos) 
    {
        return true;
    }
    return false;
    
}


void renderProcesses(processHandler& procHand)
{

    std::string searchName = inputSearchProcessesString;

    for (int i = 0; i < procHand.allPrsocesses.size(); i++)
    {

        if (procHand.allPrsocesses[i].Uid != 1000)
        {
            continue;
        }

        if (!searchName.empty() && procHand.allPrsocesses[i].name.find(inputSearchProcessesString) == std::string::npos)
        {
            continue;
        }
        
        int x = lf_get_ptr_x();
        int y = lf_get_ptr_y();
        {
            LfUIElementProps prop = lf_get_theme().text_props;
            prop.color = LF_NO_COLOR;
            prop.text_color = LF_WHITE;
            prop.hover_color = LF_RED;
            prop.hover_text_color = LF_RED;

            //prop.border_color = LF_WHITE;
            //prop.corner_radius = 5;
            //prop.margin_bottom = 20;

            lf_push_style_props(prop);

          //  lf_set_text_wrap(true);
            
            lf_text(("process name : " + procHand.allPrsocesses[i].name).c_str());
            lf_next_line();
            lf_text(("process pid  : " + std::to_string(procHand.allPrsocesses[i].pid )).c_str());
            lf_next_line();
            lf_text(("process ppid : " + std::to_string(procHand.allPrsocesses[i].ppid)).c_str());
            lf_next_line();
            lf_text(("process uid  : " + std::to_string(procHand.allPrsocesses[i].Uid)).c_str());
            lf_next_line();

            // if (lf_text_render_wchar())
            // {
            //     /* code */
            // }
            

            //lf_set_text_wrap(false);

            lf_pop_style_props();

        }

        lf_next_line();

        {
            lf_set_ptr_x_absolute(x);

            LfUIElementProps prop = lf_get_theme().button_props;

            lf_push_style_props(prop);
            //lf_button("click me :D");
            
            if(lf_button_fixed("click me :D" , 100 , -1 ) == LF_CLICKED)
            {
                kill(procHand.allPrsocesses[i].pid, SIGTERM);
                procHand.allPrsocesses.erase(procHand.allPrsocesses.begin() + i);
            }

            lf_pop_style_props();

        }
        lf_next_line();

        lf_text("---------------------\n");
        lf_next_line();
    }
        
}



int main(int argc, char *argv[])
{

    processHandler process_handler;

    glfwInit();

    GLFWwindow *window = glfwCreateWindow(width, hight, "tt", NULL, NULL);

    glfwMakeContextCurrent(window);

    lf_init_glfw(width, hight, window);

    process_handler.print();

    memset(inputSearchProcessesString, 0, 512);
    inputSearchProcesses = (LfInputField)
    {
        .width = 400,
        .buf = inputSearchProcessesString,
        .buf_size = 512,
        .placeholder = (char*)"file path "
    };

   // inputSearchProcessesString.buf_size = 512;

    while (glfwWindowShouldClose(window) == false)
    {
        process_handler.readData();
        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(0.07, 0.07, 0.07, 0.07);

        
        
        lf_begin();


            lf_div_begin((vec2s{0, 0 }) , (vec2s{width , MARGIN*3}) , false );

                {
                    LfUIElementProps props = lf_get_theme().inputfield_props;
                    props.padding = 15;
                    props.margin_right = 15;
                    props.margin_left = 15;
                    props.border_width = 0;
                    props.color = ((LfColor){30 , 30 , 30 , 30});
                    props.text_color = LF_WHITE;
                    props.border_width = 1.0f;
                    props.border_color = inputSearchProcesses.selected ? LF_WHITE : (LfColor){140, 140, 140, 255};
            
                    props.corner_radius = 1.5f;
                    props.margin_bottom = 10.0f;
                    lf_push_style_props(props);
                    lf_input_text(&inputSearchProcesses);
                    lf_pop_style_props();
                }

            lf_div_end();

        
            lf_div_begin( (vec2s{0,MARGIN*3}) , (vec2s{width , hight - MARGIN * 3}) , true );


            //for (int i = 0; i < process_handler.allPrsocesses.size(); i++)
            //{
//
            //    if (process_handler.allPrsocesses[i].Uid != 1000)
            //    {
            //        continue;
            //    }
            //    
                renderProcesses(process_handler);
            // lf_text(("process name : " + process_handler.allPrsocesses[i].name).c_str());
                // lf_next_line();
                // lf_text(("process pid  : " + std::to_string(process_handler.allPrsocesses[i].pid )).c_str());
                // lf_next_line();
                // lf_text(("process ppid : " + std::to_string(process_handler.allPrsocesses[i].ppid)).c_str());
                // lf_next_line();
                // lf_text(("process uid : " + std::to_string(process_handler.allPrsocesses[i].Uid)).c_str());
                // lf_next_line();

                // lf_text("-------------------------------------------------------");
                // lf_next_line();
            //}
            
        
           // lf_text("something went wrong :(( ");

            lf_div_end();

        lf_end();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // std::cout << "Hello world!" << std::endl;

    return 0;
}