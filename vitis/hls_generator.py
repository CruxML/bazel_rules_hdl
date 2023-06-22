import argparse
import os
import logging
import subprocess
import re
import tarfile


logger = logging.getLogger(__name__)

logging_levels = {
    "critical": logging.CRITICAL,
    "error": logging.ERROR,
    "warn": logging.WARNING,
    "warning": logging.WARNING,
    "info": logging.INFO,
    "debug": logging.DEBUG,
}


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--vitis_tcl",
        type=str,
        required=True,
        help="The path to the vitis tcl",
    )
    parser.add_argument(
        "--vitis_log",
        type=str,
        required=True,
        help="The path to the vitis log",
    )
    parser.add_argument(
        "--outputs",
        type=str,
        required=True,
        help="The path to the outputs",
    )
    parser.add_argument(
        "--label",
        type=str,
        required=True,
        help="The label name",
    )
    parser.add_argument(
        "--top_func",
        type=str,
        required=True,
        help="The top_func name",
    )
    parser.add_argument(
        "--xilinx_env",
        type=str,
        required=True,
        help="The path to the xilinx_env.",
    )
    parser.add_argument(
        "--use_vivado_hls",
        action="store_true",
        help="If use vivado hls",
    )
    parser.add_argument(
        "-l",
        "--log_level",
        default="debug",
        choices=logging_levels.keys(),
        help="The logging level to use.",
    )
    return parser.parse_args()


def send_command(command):
    build_process = subprocess.Popen(
        "/bin/bash", stdin=subprocess.PIPE, stdout=subprocess.PIPE
    )
    out, err = build_process.communicate(command.encode("utf-8"))
    logger.info(out.decode("utf-8"))
    if err:
        logger.error(err.decode("utf-8"))


def replace_module_names(verilog_files, verilog_files_dir, top_name):
    module_pattern = re.compile(r"(?<=\bmodule\s)[^\s(\n]+\b", re.IGNORECASE)
    module_names_to_change = []
    data_to_write = {}
    # Read files and find module names.
    for verilog_file in verilog_files:
        full_path = os.path.join(verilog_files_dir, verilog_file)
        with open(full_path, "r") as f:
            data_to_write[full_path] = f.readlines()
            for line in data_to_write[full_path]:
                module_name = module_pattern.findall(line)
                # Keep the top level name.
                if module_name and module_name[0] != top_name:
                    module_names_to_change += module_name
    # replace file contents
    for verilog_file in verilog_files:
        full_path = os.path.join(verilog_files_dir, verilog_file)
        this_data = data_to_write[full_path]
        for i in range(len(this_data)):
            for module_name_to_change in module_names_to_change:
                this_data[i] = re.sub(
                    r"(\b{module_name}\b(?![.]))".format(
                        module_name=module_name_to_change
                    ),
                    f" {module_name_to_change}_{top_name} ",
                    this_data[i],
                )
    for verilog_file in verilog_files:
        full_path = os.path.join(verilog_files_dir, verilog_file)
        this_data = data_to_write[full_path]
        with open(full_path, "w") as f:
            for line in this_data:
                f.write(line)


def main():
    args = parse_args()
    # TODO(cruxml-bopeng): Move this to utils_logging.
    logging.basicConfig(
        format="%(asctime)s,%(msecs)d %(levelname)-8s [%(filename)s:%(lineno)d] %(message)s",
        datefmt="%Y-%m-%d:%H:%M:%S",
        level=logging_levels[args.log_level],
    )

    if args.use_vivado_hls:
        builder_name = "vivado_hls"
    else:
        builder_name = "vitis_hls"
    build_commands = (
        f"source {args.xilinx_env}; {builder_name} {args.vitis_tcl} -l {args.vitis_log}"
    )
    send_command(build_commands)
    verilog_files_dir = os.path.join(args.label, "sol1/impl/verilog")
    verilog_files_include_dats = os.listdir(verilog_files_dir)
    verilog_files = []
    for verilog_file_include_dats in verilog_files_include_dats:
        if verilog_file_include_dats.endswith(".v"):
            verilog_files.append(verilog_file_include_dats)
    replace_module_names(verilog_files, verilog_files_dir, args.top_func)
    # Writ files
    with tarfile.open(args.outputs, "w:gz") as tar:
        for verilog_file in verilog_files_include_dats:
            full_path = os.path.join(verilog_files_dir, verilog_file)
            tar.add(full_path, arcname=os.path.basename(full_path))


if __name__ == "__main__":
    main()
