document.addEventListener('DOMContentLoaded', function () {
    fetch('getData.php') 
        .then(response => response.json())
        .then(data => {
            document.getElementById('suhumax').textContent = data.suhumax;
            document.getElementById('suhumin').textContent = data.suhumin;
            document.getElementById('suhurata').textContent = data.suhurata.toFixed(2);

            if (data.month_year_max.length > 0) {
                let monthYear = data.month_year_max[0].month_year;
                let summaryDiv = document.getElementById('summary');
                let monthYearElement = document.createElement('p');
                monthYearElement.textContent = `Bulan dan Tahun Suhu Maksimum: ${monthYear}`;
                summaryDiv.appendChild(monthYearElement);
            }

            let tableBody = document.querySelector('#data-table tbody');
            data.nilai_suhu_max_humid_max.forEach((entry, index) => {
                let row = document.createElement('tr');

                let idxCell = document.createElement('td');
                idxCell.textContent = entry.idx;
                row.appendChild(idxCell);

                let suhuCell = document.createElement('td');
                suhuCell.textContent = entry.suhu;
                row.appendChild(suhuCell);

                let humidCell = document.createElement('td');
                humidCell.textContent = entry.humid;
                row.appendChild(humidCell);

                let luxCell = document.createElement('td');
                luxCell.textContent = entry.kecerahan;
                row.appendChild(luxCell);

                let timestampCell = document.createElement('td');
                timestampCell.textContent = entry.timestamp;
                row.appendChild(timestampCell);

                tableBody.appendChild(row);
            });
        })
        .catch(error => console.error('Error fetching data:', error));
});
