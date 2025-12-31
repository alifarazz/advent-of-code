#!/usr/bin/env python
# !!!Written by AI -> Google Gemini 3.0 Fast!!!

import sys

res = 80

def generate_line_svg(input_filename, output_filename="output.svg"):
    """
    Reads x,y coordinates from a file and generates an SVG file 
    with a polyline connecting the points.
    
    Args:
        input_filename (str): The name of the input file (e.g., 'coords.txt').
        output_filename (str): The name of the output SVG file (default: 'output.svg').
    """
    
    try:
        # 1. Read the input file and collect coordinates
        with open(input_filename, 'r') as f:
            # Read all lines, strip whitespace, and filter out empty lines
            lines = [line.strip() for line in f if line.strip()]
        
        # Parse the coordinates
        points = []
        for line in lines:
            try:
                # Expecting format "x,y"
                x_str, y_str = line.split(',')
                x = float(x_str.strip())
                y = float(y_str.strip())
                points.append((x, y))
            except ValueError:
                print(f"⚠️ Warning: Skipping malformed line: '{line}'. Ensure format is 'x,y'.")
            except IndexError:
                print(f"⚠️ Warning: Skipping empty or incomplete line.")

        if not points:
            print("❌ Error: No valid coordinates found in the input file.")
            return

        # 2. Determine the SVG viewport size (optional, but good practice)
        # We'll use the bounding box of the points for the viewbox
        x_coords = [p[0] for p in points]
        y_coords = [p[1] for p in points]
        
        # Calculate min/max for viewbox
        min_x = min(x_coords)
        max_x = max(x_coords)
        min_y = min(y_coords)
        max_y = max(y_coords)

        # Add a small padding (e.g., 10 units) for better visibility
        padding = 10
        viewbox_x = min_x - padding
        viewbox_y = min_y - padding
        viewbox_width = (max_x - min_x) + (2 * padding)
        viewbox_height = (max_y - min_y) + (2 * padding)
        
        viewbox_x /= res
        viewbox_y /= res
        viewbox_width /= res
        viewbox_height /= res
        
        # Format the points string for the SVG polyline element
        points_str = " ".join([f"{x/res},{y/res}" for x, y in points])

        # 3. Generate the SVG content
        svg_content = f"""<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg width="{viewbox_width}" height="{viewbox_height}" viewBox="{viewbox_x} {viewbox_y} {viewbox_width} {viewbox_height}"
     xmlns="http://www.w3.org/2000/svg" version="1.1">
  
  <polyline points="{points_str}"
            fill="none"
            stroke="blue"
            stroke-width="2" />
            
</svg>"""

        # 4. Write the SVG content to the output file
        with open(output_filename, 'w') as f:
            f.write(svg_content)
        
        print(f"✅ Success! SVG file generated: **{output_filename}**")
        print(f"   Coordinates processed: {len(points)}")

    except FileNotFoundError:
        print(f"❌ Error: Input file not found: **{input_filename}**")
    except Exception as e:
        print(f"❌ An unexpected error occurred: {e}")

# --- Script Execution ---

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python make_svg_line.py <input_file_path>")
        print("Example: python make_svg_line.py coords.txt")
        sys.exit(1)
        
    input_file = sys.argv[1]
    # Output file is hardcoded to 'output.svg' but can be made an argument
    generate_line_svg(input_file)
