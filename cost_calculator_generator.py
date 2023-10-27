super_categories = ['Construction Cost', 'Equipment Cost', 'Operational Costs', 'Overall Cost']

categories = {
    'Construction Cost': ['Land Cost', 'Construction Cost', 'Total building'],
    'Equipment Cost': ['Server Cost', 'Storage Cost', 'Networking Cost',
                       'UPS and Power Setup Cost', 'Physical Security', 'Total equipment cost'],
    'Operational Costs': ['Personnel Cost', 'Monthly cyber security', 'Cooling and Power Costs',
                          'Maintenance and Upgrades', 'Licensing Fees', 'Other Misc Costs',
                          'Total operational costs'],
    'Overall Cost': ['Overall cost']
}

category_items = {
    'Land Cost': [('Land_area_required', 100, 5000, 1500, 50),
                  ('Land_cost_per_m2', 50, 5000, 1000, 50)],
    'Construction Cost': [('Construction_cost_per_m2', 50, 5000, 1000, 50)],
    'Total building': [],
    'Server Cost': [('N_VM', 50, 5000, 2000, 50),
                    ('VM_per_server', 1, 100, 15, 1),
                    ('Server_cost', 100, 15000, 5000, 50)],
    'Storage Cost': [('Storage_per_VM_gb', 10, 2000, 200, 10),
                     ('Cost_per_tb', 100, 5000, 1000, 50)],
    'Networking Cost': [('Networking_setup_cost', 10000, 1000000, 250000, 10000)],
    'UPS and Power Setup Cost': [('UPS_System_x2', 10000, 1000000, 200000, 10000),
                                 ('Batteries_x2', 1000, 100000, 20000, 1000),
                                 ('Generator_x2', 10000, 1000000, 200000, 10000),
                                 ('Cooling_system_integration_x2', 10000, 1000000, 100000, 10000)],
    'Physical Security': [('Access_control_sys', 1000, 100000, 20000, 1000),
                          ('CCTV_surv', 1000, 100000, 18000, 1000),
                          ('Fire_supp', 1000, 100000, 30000, 1000)],
    'Total equipment cost': [],
    'Personnel Cost': [('Num_sysadmin', 1, 20, 5, 1),
                       ('Num_network_eng', 1, 20, 3, 1),
                       ('Num_security', 1, 20, 3, 1),
                       ('Num_db_admin', 1, 20, 3, 1),
                       ('Num_support', 1, 20, 4, 1),
                       ('Num_facility_man', 1, 20, 2, 1),
                       ('Avg_monthly_salary_high', 300, 10000, 700, 100),
                       ('Avg_monthly_salary_low', 1000, 20000, 2500, 100)],
    'Monthly cyber security': [('Monthly_cyber_security', 10000, 1000000, 100000, 10000)],
    'Cooling and Power Costs': [('Power_consumption_kw', 10, 1000, 130, 10),
                                ('Electricity_price_kwh', 0.05, 1, 0.1, 0.05)],
    'Maintenance and Upgrades': [],
    'Licensing Fees': [],
    'Other Misc Costs': [],
    'Total operational costs': [],
    'Overall cost': []
}

category_intermediates = {
    'Land Cost': [('Total_land_cost', 1500000, 'prod', ['Land_area_required',
                                                        'Land_cost_per_m2'])],
    'Construction Cost': [('Total_construction_cost', 1000000, 'prod', ['Construction_cost_per_m2',
                                                                        'Land_area_required'])],
    'Total building': [('Total_building_cost', 2500000, 'sum', ['Total_land_cost',
                                                                'Total_construction_cost'])],
    'Server Cost': [('Num_servers', 134, 'ceil-div', ['VM_per_server',
                                                      'N_VM']),
                    ('Tot_srv_cost', 670000, 'prod', ['Num_servers',
                                                      'Server_cost'])],
    'Storage Cost': [('Total_storage_tb', 391, 'prod-div', ['N_VM',
                                                            'Storage_per_VM_gb']),
                     ('Total_storage_cost', 390625, 'prod', ['Cost_per_tb',
                                                             'Total_storage_tb'])],
    'Networking Cost': [],
    'UPS and Power Setup Cost': [('Total_UPS_power', 520000, 'sum', ['UPS_System_x2',
                                                                     'Batteries_x2',
                                                                     'Generator_x2',
                                                                     'Cooling_system_integration_x2'])],
    'Physical Security': [('Total_physical_sec', 68000, 'sum', ['Access_control_sys',
                                                                'CCTV_surv',
                                                                'Fire_supp'])],
    'Total equipment cost': [('Total_equipment_cost', 1898625, 'sum', ['Tot_srv_cost',
                                                                       'Total_storage_cost',
                                                                       'Networking_setup_cost',
                                                                       'Total_UPS_power',
                                                                       'Total_physical_sec'])],
    'Personnel Cost': [('Annual_personnel_cost', 470400, 'prod-sum', ['Num_sysadmin',
                                                                      'Num_network_eng',
                                                                      'Num_security',
                                                                      'Num_db_admin',
                                                                      'Avg_monthly_salary_low',
                                                                      'Num_support',
                                                                      'Num_facility_man',
                                                                      'Avg_monthly_salary_high'])],
    'Monthly cyber security': [],
    'Cooling and Power Costs': [('Monthly_power_cost', 9360, 'prod-num', ['Power_consumption_kw',
                                                                          'Electricity_price_kwh']),
                                ('Yearly_power_cost', 112320, 'prod-12', ['Monthly_power_cost'])],
    'Maintenance and Upgrades': [('Annual_maintenance', 56959, 'prod-003', ['Total_equipment_cost'])],
    'Licensing Fees': [('Annual_licensing', 56959, 'prod-003', ['Total_equipment_cost'])],
    'Other Misc Costs': [('Annual_misc_costs', 94931, 'prod-005', ['Total_equipment_cost'])],
    'Total operational costs': [('Total_operational_costs', 891569, 'sum', ['Annual_personnel_cost',
                                                                            'Monthly_cyber_security',
                                                                            'Yearly_power_cost',
                                                                            'Annual_maintenance',
                                                                            'Annual_licensing',
                                                                            'Annual_misc_costs'])],
    'Overall cost': [('Overall_cost', 5790194, 'sum', ['Total_building_cost',
                                                       'Total_equipment_cost',
                                                       'Total_operational_costs'])]
}

pre_body = '''<!DOCTYPE html>
<html>
  <head>
    <title>Cost calculator for on-premise datacenter</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        background-color: #f0f0f0;
      }

      h1 {
        color: #333;
      }

      form {
        background-color: #fff;
        border-radius: 10px;
        padding: 20px;
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);
      }

      h5 {
        color: #333;
      }

      label {
        display: block;
        margin: 10px 0;
        color: #555;
      }

      input[type="range"] {
        width: 100%;
      }

      span {
        font-weight: bold;
        font-size: 1.2em;
        color: #555;
      }

      button {
        background-color: #007bff;
        color: #fff;
        border: none;
        padding: 10px 20px;
        border-radius: 5px;
        cursor: pointer;
      }

      button:hover {
        background-color: #0056b3;
      }

      p {
        font-size: 1.2em;
        margin-top: 20px;
        color: #333;
      }
      
      .boxed-section {
            border: 2px solid #333; /* You can customize the border style */
            border-radius: 10px;
            padding: 20px;
            margin: 20px; /* Add margin for spacing */
        }
        
      .container {
          width: 50%;
          margin: 0 auto; /* Center the container horizontally on the screen */
      }
    </style>
  </head>
  <body>
  <div class="container">'''

post_body = '''
    </div>
  </body>
</html>'''

html_str = pre_body + '''
    <h1>Cost Calculator</h1>
    <form>
      '''

for super_category in super_categories:
    html_str += f'<h3>{super_category}</h3>\n'
    for category in categories[super_category]:
        html_str += '\n<div class="boxed-section">\n'
        html_str += f'<h5>{category}</h5>'
        for (item, min, max, val, step) in category_items[category]:
            item_spaced = item.replace('_', ' ')
            html_str += f'''<label for="{item}">{item_spaced}:</label>
          <input type="range" id="{item}" name="{item}" min="{min}" max="{max}" value="{val}" step="{step}">
          <span id="{item}_value">{val}</span>
          <br>
          <br>
          '''
        for (item, val, op, lst) in category_intermediates[category]:
            item_spaced = item.replace('_', ' ')
            maybe_dollar = '' if item == 'Num_servers' else '$'
            html_str += f'<p>{item_spaced}: {maybe_dollar}<span id="{item}">{val}</span></p>'
        html_str += '\n</div>\n'
    html_str += '<hr>\n'

html_str += '\n<script>\n'

for super_category in super_categories:
    for category in categories[super_category]:
        for (item, min, max, val, step) in category_items[category]:
            html_str += f'''
                    var slider_{item} = document.getElementById("{item}");
                    var output_{item} = document.getElementById("{item}_value");
                    '''
for super_category in super_categories:
    for category in categories[super_category]:
        for (item, val, op, lst) in category_intermediates[category]:
            html_str += f'''
                    var {item}_ = document.getElementById("{item}");
                    '''

html_str += '''
                function update() {'''
for super_category in super_categories:
    for category in categories[super_category]:
        for (item, min, max, val, step) in category_items[category]:
            html_str += f'''
                    var {item} = parseFloat(slider_{item}.value);\n'''
for super_category in super_categories:
    for category in categories[super_category]:
        for (item, val, op, lst) in category_intermediates[category]:
            if op == 'sum':
                html_str += f'var {item} = {'+'.join(lst)};'
            elif op == 'prod':
                html_str += f'var {item} = {'*'.join(lst)};'
            elif op == 'ceil-div':
                html_str += f'var {item} = Math.ceil(N_VM / VM_per_server);'
            elif op == 'prod-div':
                html_str += f'var {item} = N_VM * Storage_per_VM_gb / 1024;'
            elif op == 'prod-sum':
                html_str += f'''var {item} = ((Num_sysadmin + Num_network_eng + Num_security + Num_db_admin) 
                        * Avg_monthly_salary_low + (Num_support + Num_facility_man) * Avg_monthly_salary_high) * 12;'''
            elif op == 'prod-num':
                html_str += f'var {item} = Power_consumption_kw * Electricity_price_kwh * 24 * 30;'
            elif op == 'prod-12':
                html_str += f'var {item} = Monthly_power_cost * 12;'
            elif op == 'prod-003':
                html_str += f'var {item} = 0.03 * {lst[0]};'
            elif op == 'prod-005':
                html_str += f'var {item} = 0.05 * {lst[0]};'
            html_str += f'''
                        {item}_.textContent = {item};\n'''
html_str += '}\n'

for super_category in super_categories:
    for category in categories[super_category]:
        for (item, min, max, val, step) in category_items[category]:
            html_str += f'''
                                output_{item}.innerHTML = slider_{item}.value;
                                slider_{item}.oninput = function () {{
                                    output_{item}.innerHTML = this.value;
                                    update();
                                }}
                                '''
html_str += 'update();'
html_str += '\n</script>\n'

html_str += '</form>'

html_str += post_body

with open('html.html', 'w') as f:
    f.write(html_str)
