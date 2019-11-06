mod timer;
use timer::*;
use sysinfo::{SystemExt};
use libc::*;

extern{
    fn try_shutdown()->libc::c_int;
}

fn main() {
    let timer = &Timer::new(std::time::Duration::new(60,0));
    while timer.run(&mut 0,(|data:&mut dyn std::any::Any|{
            let mut system = sysinfo::System::new();
            system.refresh_all();
            let processes = system.get_process_by_name("Luminar 3.exe");
            processes.len()>0
        }
    )){
        println!("process running!");
    }
    println!("system shutdown!");
    let shutdown_result = match unsafe{try_shutdown()}{
        0 =>{"Success!"},
        1 =>{"EnablePrivileges failure!"}
        _ =>{"ExitWindowsEx API failure!"}
    };
    println!("{}",shutdown_result);
}
